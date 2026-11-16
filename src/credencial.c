#include "credencial.h"
#include <stddef.h>
#include <string.h>

int cred_formato_valido(const char *uid)
{
    size_t indice;

    if (uid == NULL || strlen(uid) != CRED_LONGITUD) {
        return 0;
    }
    for (indice = 0u; indice < CRED_LONGITUD; ++indice) {
        if (uid[indice] < '0' || uid[indice] > '9') {
            return 0;
        }
    }
    return 1;
}

int cred_checksum(const char *uid)
{
    size_t indice;
    int suma = 0;

    if (!cred_formato_valido(uid)) {
        return -1;
    }
    for (indice = 0u; indice < CRED_LONGITUD - 1u; ++indice) {
        suma += uid[indice] - '0';
    }
    return suma % 10;
}

cred_resultado_t cred_autorizar(const char *uid,
                                const char *permitidas[],
                                size_t n)
{
    size_t indice;
    int digito;

    if (!cred_formato_valido(uid)) {
        return CRED_ERR_FORMATO;
    }
    digito = cred_checksum(uid);
    if (digito < 0 || uid[CRED_LONGITUD - 1u] - '0' != digito) {
        return CRED_ERR_CHECKSUM;
    }
    if (permitidas == NULL) {
        return CRED_ERR_NO_AUTORIZADA;
    }
    for (indice = 0u; indice < n; ++indice) {
        if (permitidas[indice] != NULL && strcmp(uid, permitidas[indice]) == 0) {
            return CRED_OK;
        }
    }
    return CRED_ERR_NO_AUTORIZADA;
}

const char *cred_nombre_resultado(cred_resultado_t r)
{
    switch (r) {
    case CRED_OK:                 return "OK";
    case CRED_ERR_FORMATO:        return "ERR_FORMATO";
    case CRED_ERR_CHECKSUM:       return "ERR_CHECKSUM";
    case CRED_ERR_NO_AUTORIZADA:  return "ERR_NO_AUTORIZADA";
    default:                      return "DESCONOCIDO";
    }
}
