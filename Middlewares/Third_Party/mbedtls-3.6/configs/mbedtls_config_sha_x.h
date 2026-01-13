/**
 * @brief   Minimal configuration for SHAX Function
 * @author  mculover666
 * @date    2020/09/22
*/

#ifndef _MBEDTLS_CONFIG_SHA_X_H_
#define _MBEDTLS_CONFIG_SHA_X_H_

/* System support */
#define MBEDTLS_HAVE_ASM
//#define MBEDTLS_HAVE_TIME

/* mbed feature support */
#define MBEDTLS_NO_PLATFORM_ENTROPY

/* mbed modules */
//#define MBEDTLS_MD2_C
//#define MBEDTLS_MD4_C
//#define MBEDTLS_MD5_C
#define MBEDTLS_SHA1_C
//#define MBEDTLS_SHA224_C
#define MBEDTLS_SHA256_C
//#define MBEDTLS_SHA384_C
#define MBEDTLS_SHA512_C

#define MBEDTLS_CHECK_RETURN

//help https://blog.csdn.net/anjiyufei/article/details/135598990
#define MBEDTLS_ERROR_C   

/* AES-256-CBC */
#define MBEDTLS_CIPHER_MODE_CBC
#define MBEDTLS_AES_C
#define MBEDTLS_AES_ROM_TABLES  //Use precomputed AES tables stored in ROM

#define MBEDTLS_CIPHER_C  //cipher通用接口
#define MBEDTLS_CIPHER_PADDING_PKCS7   //在EBC模式和CBC模式中，要求输入明文长度不是分组长度的整数倍时，要对明文进行填充。


//===============


/* System support */
#define MBEDTLS_HAVE_ASM
//#define MBEDTLS_HAVE_TIME

/* mbed feature support */

#define MBEDTLS_CIPHER_MODE_CTR
#define MBEDTLS_NO_PLATFORM_ENTROPY


#define MBEDTLS_GCM_C
#define MBEDTLS_CHACHAPOLY_C


#define MBEDTLS_CHACHA20_C
#define MBEDTLS_POLY1305_C


#endif /* _MBEDTLS_CONFIG_SHA_X_H_ */
