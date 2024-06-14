//
// Created by jewoo on 2024-04-09.
//

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include <unistd.h>

#ifdef OMP
#include <omp.h>
#endif


void encoder_forward(float *out,
                     int *inp, float *wte, float *wpe,
                     int B, int T, int C) {
    for (int b = 0; b < B; ++b) {
        for (int t = 0; t < T; ++t) {
            float *out_bt = out + b * T * C + t * C;
            int ix = inp[b * T + t];
            float *wte_ix = wte + ix * C;
            float *wpe_t = wpe + t * C;

            for (int i = 0; i < C; ++i) {
                out_bt[i] = wte_ix[i] + wpe_t[i];
            }
        }
    }
}

void encode_backward(float *dwte, float *dwpe,
                     float *dout, int *inp,
                     int B, int T, int C) {
    for (int b = 0; b < B; ++b) {
        for (int t = 0; t < T; ++t) {
            float *dout_bt = dout + b * T * C + t * C;
            int ix = inp[b * T + t];
            float *dwte_ix = dwte + ix * C;
            float *dwpe_t = dwpe + t * C;

            for (int i = 0; i < C; ++i) {
                float d = dout_bt[i];
                dwte_ix[i] += d;
                dwpe_t[i] += d;
            }
        }
    }
}

void layernorm_forward(float *out, float *mean,
                       float *rstd,
                       float *inp, float *weight, float *bias,
                       int B, int T, int C) {
    float eps = 1e-5f;
    for (int b = 0; b < B; ++b) {
        for (int t = 0; t < T; ++t) {

            float *x = inp + b * T * C + t * C;

            float m = 0.0f;
            for (int i = 0; i < C; ++i) {
                m += x[i];
            }
            m /= C;
            float v = 0.0f;
            for (int i = 0; i < C; ++i) {
                v += ((x[i] - m) * (x[i] - m));
            }

            v /= C;

            float s = 1.0f / sqrtf(v + eps);

            float *out_bt = out + b * T * C + t * C;

            for (int i = 0; i < C; ++i) {
                float n = (s * (x[i] - m));
                float o = n * weight[i] + bias[i];
                out_bt[i] = o;
            }

            mean[b * T + t] = m;
            rstd[b * T + t] = s;
        }
    }
}


void layernorm_backward(float *dinp, float *dweight, float *dbias,
                        float *dout, float *inp, float *weight, float *mean,
                        float *rstd,
                        int B, int T, int C) {
    for (int b = 0; b < B; ++b) {
        for (int t = 0; t < T; ++t) {
            float *dout_bt = dout + b * T * C + t * C;
            float *inp_bt = inp + b * T * C + t * C;
            float *dinp_bt = dinp + b * T * C + t * C;
            float mean_bt = mean[b * T + t];
            float rstd_bt = rstd[b * T + t];


            float dnorm_mean = 0.0f;
            float dnorm_norm_mean = 0.0f;
            for (int i = 0; i < C; ++i) {
                float norm_bti = (inp_bt[i] - mean_bt) * rstd_bt;
                float dnorm_i = weight[i] * dout_bt[i];
                dnorm_mean += dnorm_i;
                dnorm_norm_mean += dnorm_i * norm_bti;
            }

            dnorm_mean /= C;
            dnorm_norm_mean /= C;
            for (int i = 0; i < C; ++i) {
                float norm_bti = (inp_bt[i] - mean_bt) * rstd_bt;
                float dnorm_i = weight[i] * dout_bt[i];

                dbias[i] += dout_bt[i];
                dweight[i] += norm_bti * dout_bt[i];

                float dval = 0.0f;
                dval += dnorm_i;
                dval -= dnorm_mean;
                dval -= dnorm_norm_mean * norm_bti;
                dval *= rstd_bt;
                dinp_bt[i] += dval;
            }
        }
    }
}

void matmul_forward(float *out,
                    float *inp, float *weight, float *bias,
                    int B, int T, int C, int OC) {

#pragma omp parallel for collapse(2)
    for (int b = 0; b < B; ++b) {
        for (int t = 0; t < T; ++t) {
            float *out_bt = out + b * T * OC + t * OC;
            float *inp_bt = inp + b * T * C + t * C;


            for (int o = 0; o < OC; ++o) {
                float val = (bias != NULL) ? bias[o] : 0.0f;
                float *wrow = weight + o * C;
                for (int i = 0; i < C; ++i) {
                    val += inp_bt[i] * wrow[i];
                }
                out_bt[o] = val;
            }
        }
    }
}

void matmul_backward(float *dinp, float *dweight, float *dbias,
                     float *dout, float *inp, float *weight,
                     int B, int T, int C, int OC) {
#pragma omp parallel for collapse(2)
    for (int b = 0; b < B; ++b) {
        for (int t = 0; t < T; ++t) {
            float *dout_bt = dout + b * T * OC + t * OC;
            float *dinp_bt = dinp + b * T * C + t * C;

            for (int o = 0; o < OC; ++o) {
                float *wrow = weight + o * C;
                float d = dout_bt[o];
                for (int i = 0; i < C; ++i) {
                    dinp_bt[i] += wrow[i] * d;
                }
            }
        }
    }
#pragma omp parallel for
    for (int o = 0; o < OC; ++o) {
        for (int b = 0; b < B; ++b) {
            for (int t = 0; t < T; ++t) {
                float *dout_bt = dout + b * T * OC + t * OC;
                float *int_bt = inp + b * T * C + t * C;
                float d = dout_bt[o];
                float *dweight_row = dweight + o * C;
                if (dbias != NULL) { dbias[o] += d; }
                for (int i = 0; i < C; ++i) {
                    dweight_row[i] += int_bt[i] * d;
                }
            }
        }
    }
}

void attention_forward(float *out, float *preatt, float *att,
                       float *inp, int B, int T, int C, int NH) {
    int C3 = C * 3;
    int hs = C / NH;
    float scale = 1.0 / sqrtf(hs);

#pragma omp parallel for collapse(3)
    for (int b = 0; b < B; ++b) {
        for (int t = 0; t < T; ++t) {
            for (int h = 0; h < NH; ++h) {
                float *query_t = inp + b * T * C3 + t * C3 + h * hs;
                float *preatt_bth = preatt + b * NH * T * T + h * T * T + t * T;
                float *att_bth = att + b * NH * T * T + h * T * T + t * T;

                float maxval = -10000.0f;
                for (int t2 = 0; t2 <= t; ++t2) {
                    float *key_t2 = inp + b * T * C3 + t2 * C3 + h * hs + C;

                    float val = 0.0f;
                    for (int i = 0; i < hs; ++i) {
                        val += query_t[i] * key_t2[i];
                    }
                    val *= scale;
                    if (val > maxval) {
                        maxval = val;
                    }
                    preatt_bth[t2] = val;
                }
                float expsum = 0.0f;
                for (int t2 = 0; t2 <= t; ++t2) {
                    float expv = expf(preatt_bth[t2] - maxval);
                    expsum += expv;
                    att_bth[t2] = expv;
                }

                float expsum_inv = expsum == 0.0f ? 0.0f : 1.0f / expsum;

                for (int t2 = 0; t2 < T; ++t2) {
                    if (t2 <= t) {
                        att_bth[t2] *= expsum_inv;
                    } else {
                        att_bth[t2] = 0.0f;
                    }
                }

                float *out_bth = out + b * T * C + t * C + h * hs;
                for (int i = 0; i < hs; ++i) {
                    out_bth[i] = 0.0f;
                }
                for (int t2 = 0; t2 <= t; ++t2) {
                    float *value_t2 = inp + b * T * C3 + t2 * C3 + h * hs + C * 2;
                    float att_btht2 = att_bth[t2];
                    for (int i = 0; i < hs; ++i) {
                        out_bth[i] += att_btht2 * value_t2[i];
                    }
                }
            }
        }
    }
}

void attention_backward(float *dinp, float *dpreatt, float *datt,
                        float *dout, float *inp, float *att,
                        int B, int T, int C, int NH) {
    int C3 = C * 3;
    int hs = C / NH;
    float scale = 1.0 / sqrtf(hs);

    for (int b = 0; b < B; ++b) {
        for (int t = 0; t < T; ++t) {
            for (int h = 0; h < NH; ++h) {
                float *att_bth = att + b * NH * T * T + h * T * T + t * T;
                float *datt_bth = datt + b * NH * T * T + h * T * T + t * T;
                float *dpreatt_bth = dpreatt + b * NH * T * T + h * T * T + t * T;
                float *dquery_t = dinp + b * T * C3 + t * C3 + h * hs;
                float *query_t = inp + b * T * C3 + t * C3 + h * hs;

                float *dout_bth = dout + b * T * C + t * C + h * hs;
                for (int t2 = 0; t2 <= t; ++t2) {
                    float *value_t2 = inp + b * T * C3 + t2 * C3 + h * hs + C * 2;
                    float *dvalue_t2 = dinp + b * T * C3 + t2 * C3 + h * hs + C * 2;

                    for (int i = 0; i < hs; ++i) {
                        datt_bth[t2] += value_t2[i] * dout_bth[i];
                        dvalue_t2[i] += att_bth[t2] * dout_bth[i];

                    }
                }

                for (int t2 = 0; t2 <= t; ++t2) {
                    for (int t3 = 0; t3 <= t; ++t3) {
                        float indicator = t2 == t3 ? 1.0f : 0.0f;
                        float local_derivative = att_bth[t2] * (indicator - att_bth[t3]);
                        dpreatt_bth[t3] += local_derivative * datt_bth[t2];
                    }
                }

                for (int t2 = 0; t2 <= t; ++t2) {
                    float *key_t2 = inp + b * T * C3 + t2 * C3 + h * hs + C;
                    float *dkey_t2 = dinp + b * T * C3 + t2 * C3 + h * hs + C;


                    for (int i = 0; i < hs; ++i) {
                        dquery_t[i] += key_t2[i] * dpreatt_bth[t2] * scale;
                        dkey_t2[i] += query_t[i] * dpreatt_bth[t2] * scale;
                    }
                }
            }
        }
    }
}

void gelu_forward(float *out, float *inp, int N) {
    float s = sqrtf(2.0f / M_PI);
    for (int i = 0; i < N; ++i) {
        float x = inp[i];
        float cube = 0.044715f * x * x * x;
        out[i] = 0.5f * x * (1.0f + tanhf(s * (x + cube)));
    }
}

void gelu_backward(float *dinp, float *inp, float *dout, int N) {

    float s = sqrtf(2.0f / M_PI);
    for (int i = 0; i < N; ++i) {
        float x = inp[i];
        float cube = 0.044715f * x * x * x;
        float tanh_arg = s * (x + cube);
        float tanh_out = tanhf(tanh_arg);
        float coshf_out = coshf(tanh_arg);
        float sech_out = 1.0f / (coshf_out * coshf_out);
        float local_grad = 0.5f * (1.0f + tanh_out)
                           + x * 0.5f * sech_out * s * (1.0f + 3.0f * 0.044715f * x * x);
        dinp[i] += local_grad * dout[i];
    }
}

void residual_forward(float *out, float *inp1, float *inp2, int N) {
    for (int i = 0; i < N; ++i) {
        out[i] = inp1[i] + inp2[i];
    }
}

void residual_backward(float *dinp1, float *dinp2, float *dout, int N) {
    for (int i = 0; i < N; ++i) {
        dinp1[i] += dout[i];
        dinp2[i] += dout[i];
    }
}

void softmax_forward(float *probs, float *logits, int B, int T, int V) {
#pragma omp parallel for collapse(2)
    for (int b = 0; b < B; ++b) {
        for (int t = 0; t < T; ++t) {
            float *logits_bt = logits + b * T * V + t * V;
            float *probs_bt = probs + b * T * V + t * V;

            float maxval = -10000.0f;
            for (int v = 0; v < V; ++v) {
                if (logits_bt[v] > maxval) {
                    maxval = logits_bt[v];
                }
            }

            float expsum = 0.0f;
            for (int v = 0; v < V; ++v) {
                probs_bt[v] = expf(logits_bt[v] - maxval);
                expsum += probs_bt[v];
            }
            for (int i = 0; i < V; ++i) {
                probs_bt[i] /= expsum;
            }
        }
    }
}

void crossentropy_forward(float *losses,
                          float *probs, int *targets,
                          int B, int T, int V) {
    for (int b = 0; b < B; ++b) {
        for (int t = 0; t < T; ++t) {
            float *probs_bt = probs + b * T * V + t * V;
            int target = targets[b * T + t];
            losses[b * T + t] = -logf(probs_bt[target]);
        }
    }

}

void crossentropy_softmax_backward(float *dlogits,
                                   float *dlosses, float *probs,
                                   int *targets,
                                   int B, int T, int V) {
    for (int b = 0; b < B; ++b) {
        for (int t = 0; t < T; ++t) {
            float *probs_bt = probs + b * T * V + t * V;
            float *dlogits_bt = dlogits + b * T * V + t * V;
            float dloss = dlosses[b * T + t];
            int ix = targets[b * T + t];

            for (int i = 0; i < V; ++i) {
                float p = probs_bt[i];
                float indicator = i == ix ? 1.0f : 0.0f;
                dlogits_bt[i] += (p - indicator) * dloss;
            }
        }
    }
}

#define NUM_PARAMETER_TENSORS 16
typedef struct {
    float *wte;
    float *wpe;
    float *ln1w;
    float *ln1b;
    float *qkvw;
    float *qkvb;
    float *attprojw;
    float *attprojb;
    float *ln2w;
    float *ln2b;
    float *fcw;
    float *fcb;
    float *fcprojw;
    float *fcprojb;
    float *lnfw;
    float *lnfb;
} ParameterTensors;

float *malloc_and_point_parameters(ParameterTensors *params,
                                   size_t *param_size) {
    size_t num_parameters = 0;
    for (size_t i = 0; i < NUM_PARAMETER_TENSORS; ++i) {
        num_parameters += param_size[i];
    }

    float *param_memory = (float *) malloc(num_parameters * sizeof(float));
    float **ptrs[] = {
            &params->wte, &params->wpe,
            &params->ln1w, &params->ln1b,
            &params->qkvw, &params->qkvb,
            &params->attprojw, &params->attprojb,
            &params->ln2w, &params->ln2b,
            &params->fcw, &params->fcb,
            &params->fcprojw, &params->fcprojb,
            &params->lnfw, &params->lnfb
    };
    float *prams_memory_iterator = param_memory;
    for (size_t i = 0; i < NUM_PARAMETER_TENSORS; ++i) {
        *(ptrs[i]) = prams_memory_iterator;
        prams_memory_iterator += param_size[i];
    }
    return param_memory;
}

#define NUM_ACTIVATION_TENSORS 23
typedef struct {
    float *encoded;
    float *ln1;
    float *ln1_mean;
    float *ln1_rstd;
    float *qkv;
    float *atty;
    float *preatt;
    float *att;
    float *attproj;
    float *residual2;
    float *ln2;
    float *ln2_mean;
    float *ln2_rstd;
    float *fch;
    float *fch_gelu;
    float *fcproj;
    float *residual3;
    float *lnf;
    float *lnf_mean;
    float *lnf_rstd;
    float *logits;
    float *probs;
    float *losses;
} ActivationTensors;

float *malloc_and_point_activations(ActivationTensors *acts,
                                    size_t *act_sizes) {
    size_t num_activations = 0;
    for (size_t i = 0; i < NUM_ACTIVATION_TENSORS; ++i) {
        num_activations += act_sizes[i];
    }

    float *act_memory = (float *) malloc(num_activations * sizeof(float));
    float **ptrs[] = {
            &acts->encoded, &acts->ln1, &acts->ln1_mean, &acts->ln1_rstd,
            &acts->qkv, &acts->atty, &acts->preatt, &acts->att, &acts->attproj,
            &acts->residual2, &acts->ln2, &acts->ln2_mean, &acts->ln2_rstd,
            &acts->fch, &acts->fch_gelu, &acts->fcproj, &acts->residual3,
            &acts->lnf, &acts->lnf_mean, &acts->lnf_rstd,
            &acts->logits, &acts->probs, &acts->losses
    };
    float *act_memory_iterator = act_memory;
    for (size_t i = 0; i < NUM_ACTIVATION_TENSORS; ++i) {
        *(ptrs[i]) = act_memory_iterator;
        act_memory_iterator += act_sizes[i];
    }
    return act_memory;
}

typedef struct {
    int max_seq_len;
    int vocab_size;
    int num_layers;
    int num_heads;
    int channels;
} GPT2Config;

typedef struct {
    GPT2Config config;
    ParameterTensors params;
    size_t param_sizes[NUM_PARAMETER_TENSORS];
    float *params_memory;
    int num_parameter;

    ParameterTensors grads;
    float *grads_memory;

    float *m_memory;
    float *v_memory;

    ActivationTensors acts;
    size_t act_sizes[NUM_ACTIVATION_TENSORS];
    float *acts_memory;
    int num_activations;

    ActivationTensors grads_acts;
    float *grads_acts_memory;

    int batch_size;
    int seq_len;
    int *inputs;
    int *targets;
    float mean_loss;
} GPT2;

void gpt2_build_from_checkpoint(GPT2 *model, char *checkpoint_path) {

    FILE *model_file = fopen(checkpoint_path, "rb");
    if (model_file == NULL) {
        printf("Error opening file!\n");
        exit(1);
    }
    int model_header[256];
    fread(model_header, sizeof(int), 256, model_file);
    if (model_header[0] != 20240326) {
        printf("Bad Magic Model File!\n");
        exit(1);
    }
    if (model_header[1] != 1) {
        printf("Invalid model version!\n");
        exit(1);
    }

    int maxT, V, L, NH, C;
    model->config.max_seq_len = maxT = model_header[2];
    model->config.vocab_size = V = model_header[3];
    model->config.num_layers = L = model_header[4];
    model->config.num_heads = NH = model_header[5];
    model->config.channels = C = model_header[6];
    printf("[GPT-2]\n");
    printf("max_seq_len = %d\n", maxT);
    printf("vocab_size = %d\n", V);
    printf("num_layers = %d\n", L);
    printf("num_heads = %d\n", NH);
    printf("channels = %d\n", C);

    model->param_sizes[0] = V * C;
    model->param_sizes[1] = maxT * V;
    model->param_sizes[2] = L * C;
    model->param_sizes[3] = L * C;
    model->param_sizes[4] = L * (3 * C) * C;
    model->param_sizes[5] = L * (3 * C);
    model->param_sizes[6] = L * C * C;
    model->param_sizes[7] = L * C;
    model->param_sizes[8] = L * C;
    model->param_sizes[9] = L * C;
    model->param_sizes[10] = L * (4 * C) * C;
    model->param_sizes[11] = L * (4 * C);
    model->param_sizes[12] = L * C * (4 * C);
    model->param_sizes[13] = L * C;
    model->param_sizes[14] = C;
    model->param_sizes[15] = C;


    size_t num_parameters = 0;
    for (size_t i = 0; i < NUM_PARAMETER_TENSORS; ++i) {
        num_parameters += model->param_sizes[i];
    }
    printf("num_parameters = %zu\n", num_parameters);
    model->num_parameter = num_parameters;

    model->params_memory = malloc_and_point_parameters(&model->params, model->param_sizes);
    fread(model->params_memory, sizeof(float), num_parameters, model_file);
    fclose(model_file);


    model->acts_memory = NULL;
    model->grads_memory = NULL;
    model->m_memory = NULL;
    model->v_memory = NULL;
    model->grads_acts_memory = NULL;
    model->inputs = NULL;
    model->targets = NULL;
    model->batch_size = 0;
    model->seq_len = 0;
    model->mean_loss = -1.0f;
}

void gpt2_forward(GPT2 *model, int *inputs,
                  int *targets, int B, int T) {
    if (model->params_memory == NULL) {
        printf("Model not initialized!\n");
        exit(1);
    }
    int V = model->config.vocab_size;
    int L = model->config.num_layers;
    int NH = model->config.num_heads;
    int C = model->config.channels;

    if (model->acts_memory == NULL) {
        model->batch_size = 8;
        model->seq_len = T;
        model->act_sizes[0] = B * T * C;
        model->act_sizes[1] = L * B * T * C;
        model->act_sizes[2] = L * B * T;
        model->act_sizes[3] = L * B * T;
        model->act_sizes[4] = L * B * T * 3 * C;
        model->act_sizes[5] = L * B * T * C;
        model->act_sizes[6] = L * B * NH * T * C;
        model->act_sizes[7] = L * B * NH * T * C;
        model->act_sizes[8] = L * B * T * C;
        model->act_sizes[9] = L * B * T * C;
        model->act_sizes[10] = L * B * T * C;
        model->act_sizes[11] = L * B * T;
        model->act_sizes[12] = L * B * T;
        model->act_sizes[13] = L * B * T * 4 * C;
        model->act_sizes[14] = L * B * T * 4 * C;
        model->act_sizes[15] = L * B * T * C;
        model->act_sizes[16] = L * B * T * C;
        model->act_sizes[17] = L * B * T;
        model->act_sizes[18] = B * T;
        model->act_sizes[19] = B * T;
        model->act_sizes[20] = B * T * V;
        model->act_sizes[21] = B * T * V;
        model->act_sizes[22] = B * T;
        size_t num_activations = 0;
        for (size_t i = 0; i < NUM_ACTIVATION_TENSORS; ++i) {
            num_activations += model->act_sizes[i];
        }

        printf("num_activations: %zu:\n", num_activations);
        model->num_activations = num_activations;
        model->acts_memory = malloc_and_point_activations(&model->acts, model->act_sizes);
        model->inputs = (int *) malloc(B * T * sizeof(int));
        model->targets = (int *) malloc(B * T * sizeof(int));
    } else {
        if (B > model->batch_size || T > model->seq_len) {
            printf("Invalid batch size or sequence length!\n");
            printf("Model: B=%d T=%d, Desired: B=%d T=%d\n", model->batch_size, model->seq_len, B, T);
            exit(1);
        }
    }

    memcpy(model->inputs, inputs, B * T * sizeof(int));
    if (targets == NULL) {
        memcpy(model->targets, inputs, B * T * sizeof(int));
    }

    ParameterTensors params = model->params;
    ActivationTensors acts = model->acts;
    float *residual;
    encoder_forward(acts.encoded, inputs,
                    params.wte, params.wpe, B, T, C);
    for (int l = 0; l < L; ++l) {
        residual = l == 0 ? acts.encoded : acts.residual3 + (l - 1) * B * T * C;

        float *l_ln1w = params.ln1w + l * C;
        float *l_ln1b = params.ln1b + l * C;
        float *l_ln2w = params.ln2w + l * C;
        float *l_ln2b = params.ln2b + l * C;
        float *l_qkvw = params.qkvw + l * (3 * C) * C;
        float *l_qkvb = params.qkvb + l * (3 * C);
        float *l_attprojw = params.attprojw + l * C * C;
        float *l_attprojb = params.attprojb + l * C;
        float *l_fcw = params.fcw + l * 4 * C * C;
        float *l_fcb = params.fcb + l * 4 * C;
        float *l_fcprojw = params.fcprojw + l * C * 4 * C;
        float *l_fcprojb = params.fcprojb + l * C;

        float *l_ln1 = acts.ln1 + l * B * T * C;
        float *l_ln2 = acts.ln2 + l * B * T * C;
        float *l_ln1_mean = acts.ln1_mean + l * B * T;
        float *l_ln1_rstd = acts.ln1_rstd + l * B * T;
        float *ln2_mean = acts.ln2_mean + l * B * T;
        float *ln2_rstd = acts.ln2_rstd + l * B * T;

        float *l_qkv = acts.qkv + l * B * T * 3 * C;
        float *l_atty = acts.atty + l * B * T * C;
        float *l_preatt = acts.preatt + l * B * NH * T * T;
        float *l_att = acts.att + l * B * NH * T * T;
        float *l_attproj = acts.attproj + l * B * T * C;
        float *l_residual2 = acts.residual2 + l * B * T * C;
        float *l_fch = acts.fch + l * B * T * 4 * C;
        float *l_fcproj = acts.fcproj + l * B * T * C;
        float *l_residual3 = acts.residual3 + l * B * T * C;
        float *l_fch_gelu = acts.fch_gelu + l * B * T * 4 * C;

        layernorm_forward(l_ln1, l_ln1_mean, l_ln1_rstd, residual,
                          l_ln1w, l_ln1b, B, T, C);
        matmul_forward(l_qkv, l_ln1, l_qkvw, l_qkvb, B, T, C, 3 * C);
        attention_forward(l_atty, l_preatt,
                          l_att, l_qkv, B, T, C, NH);
        matmul_forward(l_attproj, l_atty, l_attprojw, l_attprojb, B, T, C, C);
        residual_forward(l_residual2, residual, l_attproj, B * T * C);
        layernorm_forward(l_ln2, ln2_mean, ln2_rstd, l_residual2,
                          l_ln2w, l_ln2b, B, T, C);
        matmul_forward(l_fch, l_ln2, l_fcw, l_fcb, B, T, C, 4 * C);
        gelu_forward(l_fch_gelu, l_fch, B * T * C * 4);
        matmul_forward(l_fcproj, l_fch_gelu, l_fcprojw, l_fcprojb, B, T, 4 * C, C);
        residual_forward(l_residual3, l_residual2, l_fcproj, B * T * C);
    }

    residual = acts.residual3 + (L - 1) * B * T * C;
    layernorm_forward(acts.lnf, acts.lnf_mean, acts.lnf_rstd, residual,
                      params.lnfw, params.lnfb, B, T, C);
    matmul_forward(acts.logits, acts.lnf, params.wte, NULL, B, T, C, V);
    softmax_forward(acts.probs, acts.logits, B, T, V);

    if (targets != NULL) {
        crossentropy_forward(model->acts.losses,
                             model->acts.probs,
                             targets, B, T, V);
        float mean_loss = 0.0f;
        for (int i = 0; i < B * T; ++i) {
            mean_loss += model->acts.losses[i];
        }
        mean_loss /= B * T;
        model->mean_loss = mean_loss;
    } else {
        model->mean_loss = -1.0f;
    }
}

void gpt2_zero_grad(GPT2 *model) {
    if (model->grads_memory != NULL) {
        memset(model->grads_memory, 0, model->num_parameter * sizeof(float));
    }
    if (model->grads_acts_memory != NULL) {
        memset(model->grads_acts_memory, 0, model->num_activations * sizeof(float));
    }
}

void gpt2_backward(GPT2 *model) {
    if (model->mean_loss == -1.0f) {
        printf("No loss computed!\n");
        exit(1);
    }

    if (model->grads_memory == NULL) {
        model->grads_memory = malloc_and_point_activations(&model->grads, model->param_sizes);
        model->grads_acts_memory = malloc_and_point_activations(&model->grads_acts, model->act_sizes);
        gpt2_zero_grad(model);
    }

    int B = model->batch_size;
    int V = model->config.vocab_size;
    int T = model->seq_len;
    int L = model->config.num_layers;
    int NH = model->config.num_heads;
    int C = model->config.channels;

    ParameterTensors params = model->params;
    ParameterTensors grads = model->grads;
    ActivationTensors acts = model->acts;
    ActivationTensors grads_acts = model->grads_acts;

    float dloss_mean = 1.0f / (B * T);
    for (int i = 0; i < B * T; ++i) {
        grads_acts.losses[i] = dloss_mean;
    }
    crossentropy_softmax_backward(grads_acts.logits,
                                  grads_acts.losses,
                                  acts.probs, model->targets, B, T, V);
    matmul_backward(grads_acts.lnf,
                    grads.wte, NULL,
                    grads_acts.logits,
                    acts.lnf, params.wte, B, T, C, V);
    float *residual = acts.residual3 + (L - 1) * B * T * C;
    float *dresidual = grads_acts.residual3 + (L - 1) * B * T * C;

    layernorm_backward(dresidual,
                       grads.lnfw, grads.lnfb,
                       grads_acts.lnf, residual, params.lnfw,
                       acts.lnf_mean, acts.lnf_rstd, B, T, C);

    for (int l = L - 1; l >= 0; l--) {
        residual = l == 0 ? acts.encoded : acts.residual3 + (l - 1) * B * T * C;
        dresidual = l == 0 ? grads_acts.encoded : grads_acts.residual3 + (l - 1) * B * T * C;

        float *l_ln1w = params.ln1w + l * C;
        float *l_qkvw = params.qkvw + l * (3 * C) * C;
        float *l_attprojw = params.attprojw + l * C * C;
        float *l_ln2w = params.ln2w + l * C;
        float *l_fcw = params.fcw + l * 4 * C * C;
        float *l_fcprojw = params.fcprojw + l * C * 4 * C;

        float *dl_ln1w = grads.ln1w + l * C;
        float *dl_ln1b = grads.ln1b + l * C;
        float *dl_qkvw = grads.qkvw + l * (3 * C) * C;
        float *dl_qkvb = grads.qkvb + l * (3 * C);
        float *dl_attprojw = grads.attprojw + l * C * C;
        float *dl_attprojb = grads.attprojw + l * C;
        float *dl_ln2w = grads.ln2w + l * C;
        float *dl_ln2b = grads.ln2b + l * C;
        float *dl_fcw = grads.fcw + l * 4 * C * C;
        float *dl_fcb = grads.fcb + l * 4 * C;
        float *dl_fcprojw = grads.fcprojw + l * C * 4 * C;
        float *dl_fcprojb = grads.fcprojb + l * C;

        float *l_ln1 = acts.ln1 + l * B * T * C;
        float *l_ln1_mean = acts.ln1_mean + l * B * T;
        float *l_ln1_rstd = acts.ln1_rstd + l * B * T;
        float *l_qkv = acts.qkv + l * B * T * 3 * C;
        float *l_atty = acts.atty + l * B * T * C;
        float *l_att = acts.att + l * B * NH * T * T;
        float *l_residual2 = acts.residual2 + l * B * T * C;
        float *l_ln2 = acts.ln2 + l * B * T * C;
        float *l_ln2_mean = acts.ln2_mean + l * B * T;
        float *l_ln2_rstd = acts.ln2_rstd + l * B * T;
        float *l_fch = acts.fch + l * B * T * 4 * C;
        float *l_fch_gelu = acts.fch_gelu + l * B * T * 4 * C;

        float *dl_ln1 = grads_acts.ln1 + l * B * T * C;
        float *dl_qkv = grads_acts.qkv + l * B * T * 3 * C;
        float *dl_atty = grads_acts.atty + l * B * T * C;
        float *dl_preatt = grads_acts.preatt + l * B * NH * T * T;
        float *dl_att = grads_acts.att + l * B * NH * T * T;
        float *dl_attproj = grads_acts.attproj + l * B * T * C;
        float *dl_residual2 = grads_acts.residual2 + l * B * T * C;
        float *dl_ln2 = grads_acts.ln2 + l * B * T * C;
        float *dl_fch = grads_acts.fch + l * B * T * 4 * C;
        float *dl_fch_gelu = grads_acts.fch_gelu + l * B * T * 4 * C;
        float *dl_fcproj = grads_acts.fcproj + l * B * T * C;
        float *dl_residual3 = grads_acts.residual3 + l * B * T * C;


        residual_backward(dl_residual2, dl_fcproj, dl_residual3, B * T * C);
        matmul_backward(dl_fch_gelu, dl_fcprojw,
                        dl_fcprojb, dl_fcproj,
                        l_fch_gelu, l_fcprojw, B, T, 4 * C, C);
        gelu_backward(dl_fch, l_fch, dl_fch_gelu, B * T * 4 * C);
        matmul_backward(dl_ln2, dl_fcw,
                        dl_fcb, dl_fch,
                        l_ln2, l_fcw, B, T, C, C * 4);
        layernorm_backward(dl_residual2, dl_ln2w, dl_ln2b,
                           dl_ln2, l_residual2, l_ln2w,
                           l_ln2_mean, l_ln2_rstd, B, T, C);
        residual_forward(dresidual,
                         dl_attproj, dl_residual2, B * T * C);
        matmul_backward(dl_atty, dl_attprojw, dl_attprojb, dl_attproj, l_atty, l_attprojw, B, T, C, C);
        attention_backward(dl_qkv, dl_preatt, dl_att, dl_atty, l_qkv, l_att, B, T, C, NH);
        matmul_backward(dl_ln1,
                        dl_qkvw, dl_qkvb, dl_qkv, l_ln1, l_qkvw, B, T, C, 3 * C);
        layernorm_backward(dresidual, dl_ln1w, dl_ln1b,
                           dl_ln1, residual, l_ln1w,
                           l_ln1_mean, l_ln1_rstd, B, T, C);
    }
    encode_backward(grads.wte, grads.wpe,
                    grads_acts.encoded,
                    model->inputs, B, T, C);
}

void gpt2_update(GPT2 *model, float learning_rate,
                 float beta1, float beta2, float epsilon, float weight_decay, int t) {
    if (model->m_memory == NULL) {
        model->v_memory = (float *) malloc(model->num_parameter * sizeof(float));
        model->m_memory = (float *) malloc(model->num_parameter * sizeof(float));
    }
    for (int i = 0; i < model->num_parameter; ++i) {
        float param = model->params_memory[i];
        float grad = model->grads_memory[i];

        float m = model->m_memory[i] * beta1 + (1.0f - beta1) * grad;
        float v = model->v_memory[i] * beta2 + (1.0f - beta2) * grad * grad;
        float m_hat = m / (1.0f - powf(beta1, t));
        float v_hat = v / (1.0f - powf(beta2, t));

        model->v_memory[i] = v;
        model->m_memory[i] = m;
        model->params_memory[i] -= learning_rate * (m_hat / (sqrtf(v_hat) + epsilon) + weight_decay * param);

    }
}

void gpt2_free(GPT2 *model) {
    free(model->params_memory);
    free(model->grads_memory);
    free(model->m_memory);
    free(model->v_memory);
    free(model->acts_memory);
    free(model->grads_acts_memory);
    free(model->inputs);
    free(model->targets);

}

#ifndef TESTING
typedef struct {
    int B;
    int T;

    FILE *tokens_file;
    long file_size;
    long current_position;
    int *batch;
    int *inputs;
    int *targets;
    int num_batches;
} DataLoader;

void dataloader_init(DataLoader *loader, char *filename, int B, int T) {
    loader->B = B;
    loader->T = T;
    loader->tokens_file = fopen(filename, "rb");
    if (loader->tokens_file == NULL) {
        printf("Failed to open file %s\n", filename);
        exit(1);
    }
    fseek(loader->tokens_file, 0L, SEEK_END);
    loader->file_size = ftell(loader->tokens_file);
    fseek(loader->tokens_file, 0L, SEEK_SET);
    if (loader->file_size < (B * T + 1) * sizeof(int)) {
        printf("File size is not a multiple of %d * %d * sizeof(int)\n", B, T);
        exit(1);
    }
    loader->current_position = 0;
    loader->batch = (int *) malloc((B * T + 1) * sizeof(int));
    loader->inputs = loader->batch;
    loader->targets = loader->batch + 1;
    loader->num_batches = loader->file_size / (B * T * sizeof(int));
}

void dataloader_reset(DataLoader *loader) {
    loader->current_position = 0;
}

void dataloader_next_batch(DataLoader *loader) {
    int B = loader->B;
    int T = loader->T;
    if (loader->current_position + (B * T + 1) * sizeof(int) > loader->file_size) {
        dataloader_reset(loader);
    }

    fseek(loader->tokens_file, loader->current_position, SEEK_SET);
    fread(loader->batch, sizeof(int), B * T + 1, loader->tokens_file);
    loader->current_position += B * T * sizeof(int);
}

void dataloader_free(DataLoader *loader) {
    fclose(loader->tokens_file);
    free(loader->batch);
}

#define GPT2_EOT 50256

unsigned int random_u32(unsigned long long *state) {
    *state ^= *state >> 12;
    *state ^= *state << 25;
    *state ^= *state >> 27;
    return (*state * 0x2545F4914F6CDD1Dull) >> 32;
}

float random_f32(unsigned long long *state) {
    return (random_u32(state) >> 8) / 16777216.0f;
}

int sample_mult(float *probabilites, int n, float coin) {
    float cdf = 0.0f;
    for (int i = 0; i < n; ++i) {
        cdf += probabilites[i];
        if (cdf > coin) {
            return i;
        }
    }
    return n - 1;
}

int main() {
    GPT2 model;
    gpt2_build_from_checkpoint(&model, "gpt2_124M.bin");

    char *tiny_stories_train = "data/TinyStories_train.bin";
    char *tiny_stories_val = "data/TinyStories_val.bin";
    char *tiny_shakespeare_train = "data/TinyShakespeare_train.bin";
    char *tiny_shakespeare_val = "data/TinyShakespeare_val.bin";

    char *train_tokens = access(tiny_shakespeare_train, F_OK) != -1 ? tiny_shakespeare_train : tiny_stories_train;
    char *val_tokens = access(tiny_shakespeare_val, F_OK) != -1 ? tiny_shakespeare_val : tiny_stories_val;

    int B = 4;
    int T = 64;
    DataLoader train_loader;
    dataloader_init(&train_loader, train_tokens, B, T);
    printf("train dataset num_batches: %d\n", train_loader.num_batches);
    DataLoader val_loader;
    dataloader_init(&val_loader, val_tokens, B, T);
    printf("val dataset num_batches: %d\n", val_loader.num_batches);
    int val_num_batches = 10;

    unsigned long long rng_state = 1337;
    const int gen_max_length = 64;
    int gen_tokens[gen_max_length];

    struct timespec start, end;
    for (int step = 0; step <= 40; ++step) {

        if (step % 10 == 0) {
            float val_loss = 0.0f;
            dataloader_reset(&val_loader);
            for (int i = 0; i < val_num_batches; ++i) {
                dataloader_next_batch(&val_loader);
                gpt2_forward(&model, val_loader.inputs, val_loader.targets, B, T);
                val_loss += model.mean_loss;
            }
            val_loss /= val_num_batches;
            printf("val loss: %f\n", val_loss);
        }
        if (step > 0 && step % 20 == 0) {
            gen_tokens[0] = GPT2_EOT;
            for (int t = 1; t < gen_max_length; ++t) {
                gpt2_forward(&model, gen_tokens, NULL, 1, t);

                float *probs = model.acts_memory + (t - 1) * model.config.vocab_size;
                float coin = random_f32(&rng_state);
                int next_token = sample_mult(probs, model.config.vocab_size, coin);
                gen_tokens[t] = next_token;
            }
            printf("generated: ");
            for (int i = 0; i < gen_max_length; ++i) {
                printf("%d ", gen_tokens[i]);
            }
            printf("\n");
        }

        clock_gettime(CLOCK_MONOTONIC, &start);
        dataloader_next_batch(&train_loader);
        gpt2_forward(&model, train_loader.inputs, train_loader.targets, B, T);
        gpt2_zero_grad(&model);
        gpt2_backward(&model);
        gpt2_update(&model, 1e-4f, 0.9f, 0.999f, 1e-8f, 0.0f, step + 1);
        clock_gettime(CLOCK_MONOTONIC, &end);
        double time_elapsed_s = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
        printf("time elapsed: %f ms\n", time_elapsed_s * 1000);
        printf("loss: %f\n", model.mean_loss);
    }

    dataloader_free(&train_loader);
    dataloader_free(&val_loader);
    gpt2_free(&model);
    return 0;
}

#endif