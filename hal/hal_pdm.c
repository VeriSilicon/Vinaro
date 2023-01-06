#include <string.h>
#include "vsd_error.h"
#include "hal_pdm.h"

#ifndef ARRAY_SIZE
#define ARRAY_SIZE(a) (sizeof(a) / sizeof((a)[0]))
#endif

static PdmDevice *g_pdm_dev[PDM_PORT_MAX] = { NULL };

static inline PdmOperation *get_ops(const PdmDevice *pdm)
{
    return (PdmOperation *)pdm->ops;
}

int hal_pdm_add_dev(PdmDevice *pdm)
{
    int ret   = VSD_ERR_FULL;
    uint8_t i = 0;

    for (i = 0; i < ARRAY_SIZE(g_pdm_dev); i++) {
        if (g_pdm_dev[i] == NULL) {
            g_pdm_dev[i] = pdm;
            ret          = VSD_SUCCESS;
            break;
        }
    }

    return ret;
}

int hal_pdm_remove_dev(PdmDevice *pdm)
{
    int ret   = VSD_ERR_NON_EXIST;
    uint8_t i = 0;

    for (i = 0; i < ARRAY_SIZE(g_pdm_dev); i++) {
        if (g_pdm_dev[i] == pdm) {
            g_pdm_dev[i] = NULL;
            ret          = VSD_SUCCESS;
            break;
        }
    }

    return ret;
}

PdmDevice *hal_pdm_get_device(uint8_t dev_id)
{
    uint8_t i;

    for (i = 0; i < ARRAY_SIZE(g_pdm_dev); i++) {
        if (g_pdm_dev[i]->hw_config && g_pdm_dev[i]->hw_config->id == dev_id)
            return g_pdm_dev[i];
    }

    return NULL;
}

int hal_pdm_init(const PdmDevice *pdm)
{
    if (get_ops(pdm)->init)
        return get_ops(pdm)->init(pdm);

    return VSD_SUCCESS;
}

int hal_pdm_start(const PdmDevice *pdm, PdmSubstream *stream)
{
    if (get_ops(pdm)->start)
        return get_ops(pdm)->start(pdm, stream);

    return VSD_SUCCESS;
}

void hal_pdm_stop(const PdmDevice *pdm, PdmSubstream *stream)
{
    if (get_ops(pdm)->stop)
        get_ops(pdm)->stop(pdm, stream);
}

uint8_t hal_pdm_get_rx_status(const PdmDevice *pdm)
{
    if (get_ops(pdm)->get_rx_status)
        return get_ops(pdm)->get_rx_status(pdm);

    return 0;
}

void hal_pdm_clear_status(const PdmDevice *pdm)
{
    if (get_ops(pdm)->clear_staus)
        get_ops(pdm)->clear_staus(pdm);
}

int hal_pdm_finalize(const PdmDevice *pdm)
{
    if (get_ops(pdm)->deinit)
        get_ops(pdm)->deinit(pdm);

    return VSD_SUCCESS;
}

DRV_ISR_SECTION
void hal_pdm_irq_handler(const PdmDevice *pdm)
{
    if (!pdm || !get_ops(pdm)->irq_handler)
        return;

    get_ops(pdm)->irq_handler(pdm);
}
