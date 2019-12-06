/*
 * Copyright 2010-2019 Amazon.com, Inc. or its affiliates. All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License").
 * You may not use this file except in compliance with the License.
 * A copy of the License is located at
 *
 *  http://aws.amazon.com/apache2.0
 *
 * or in the "license" file accompanying this file. This file is distributed
 * on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either
 * express or implied. See the License for the specific language governing
 * permissions and limitations under the License.
 */
#include <aws/cal/ecc.h>
#include <aws/cal/hash.h>
#include <aws/common/byte_buf.h>
#include <aws/testing/aws_test_harness.h>

static int s_ecdsa_p256_test_pub_key_derivation_fn(struct aws_allocator *allocator, void *ctx) {
    (void)ctx;

    uint8_t d[] = {
        0xc9, 0x80, 0x68, 0x98, 0xa0, 0x33, 0x49, 0x16, 0xc8, 0x60, 0x74, 0x88, 0x80, 0xa5, 0x41, 0xf0,
        0x93, 0xb5, 0x79, 0xa9, 0xb1, 0xf3, 0x29, 0x34, 0xd8, 0x6c, 0x36, 0x3c, 0x39, 0x80, 0x03, 0x57,
    };

    struct aws_byte_cursor private_key = aws_byte_cursor_from_array(d, sizeof(d));

    struct aws_ecc_key_pair *derived_key =
        aws_ecc_key_pair_new_derived_from_private_key(allocator, AWS_CAL_ECDSA_P256, &private_key);

    ASSERT_NOT_NULL(derived_key);

    uint8_t x[] = {
        0xd0, 0x72, 0x0d, 0xc6, 0x91, 0xaa, 0x80, 0x09, 0x6b, 0xa3, 0x2f, 0xed, 0x1c, 0xb9, 0x7c, 0x2b,
        0x62, 0x06, 0x90, 0xd0, 0x6d, 0xe0, 0x31, 0x7b, 0x86, 0x18, 0xd5, 0xce, 0x65, 0xeb, 0x72, 0x8f,
    };
    ASSERT_BIN_ARRAYS_EQUALS(x, sizeof(x), derived_key->pub_x.buffer, derived_key->pub_x.len);

    uint8_t y[] = {
        0x96, 0x81, 0xb5, 0x17, 0xb1, 0xcd, 0xa1, 0x7d, 0x0d, 0x83, 0xd3, 0x35, 0xd9, 0xc4, 0xa8, 0xa9,
        0xa9, 0xb0, 0xb1, 0xb3, 0xc7, 0x10, 0x6d, 0x8f, 0x3c, 0x72, 0xbc, 0x50, 0x93, 0xdc, 0x27, 0x5f,
    };
    ASSERT_BIN_ARRAYS_EQUALS(y, sizeof(y), derived_key->pub_y.buffer, derived_key->pub_y.len);

    aws_ecc_key_pair_destroy(derived_key);

    return AWS_OP_SUCCESS;
}

AWS_TEST_CASE(ecdsa_p256_test_pub_key_derivation, s_ecdsa_p256_test_pub_key_derivation_fn)

static int s_ecdsa_p256_test_known_signing_value_fn(struct aws_allocator *allocator, void *ctx) {
    (void)ctx;

    uint8_t d[] = {
        0x51, 0x9b, 0x42, 0x3d, 0x71, 0x5f, 0x8b, 0x58, 0x1f, 0x4f, 0xa8, 0xee, 0x59, 0xf4, 0x77, 0x1a,
        0x5b, 0x44, 0xc8, 0x13, 0x0b, 0x4e, 0x3e, 0xac, 0xca, 0x54, 0xa5, 0x6d, 0xda, 0x72, 0xb4, 0x64,
    };

    struct aws_byte_cursor private_key = aws_byte_cursor_from_array(d, sizeof(d));

    struct aws_ecc_key_pair *signing_key =
        aws_ecc_key_pair_new_derived_from_private_key(allocator, AWS_CAL_ECDSA_P256, &private_key);

    struct aws_byte_cursor x = aws_byte_cursor_from_buf(&signing_key->pub_x);
    struct aws_byte_cursor y = aws_byte_cursor_from_buf(&signing_key->pub_y);

    struct aws_ecc_key_pair *verifying_key =
        aws_ecc_key_pair_new_from_public_key(allocator, AWS_CAL_ECDSA_P256, &x, &y);

    uint8_t message[] = {
        0x59, 0x05, 0x23, 0x88, 0x77, 0xc7, 0x74, 0x21, 0xf7, 0x3e, 0x43, 0xee, 0x3d, 0xa6, 0xf2, 0xd9,
        0xe2, 0xcc, 0xad, 0x5f, 0xc9, 0x42, 0xdc, 0xec, 0x0c, 0xbd, 0x25, 0x48, 0x29, 0x35, 0xfa, 0xaf,
        0x41, 0x69, 0x83, 0xfe, 0x16, 0x5b, 0x1a, 0x04, 0x5e, 0xe2, 0xbc, 0xd2, 0xe6, 0xdc, 0xa3, 0xbd,
        0xf4, 0x6c, 0x43, 0x10, 0xa7, 0x46, 0x1f, 0x9a, 0x37, 0x96, 0x0c, 0xa6, 0x72, 0xd3, 0xfe, 0xb5,
        0x47, 0x3e, 0x25, 0x36, 0x05, 0xfb, 0x1d, 0xdf, 0xd2, 0x80, 0x65, 0xb5, 0x3c, 0xb5, 0x85, 0x8a,
        0x8a, 0xd2, 0x81, 0x75, 0xbf, 0x9b, 0xd3, 0x86, 0xa5, 0xe4, 0x71, 0xea, 0x7a, 0x65, 0xc1, 0x7c,
        0xc9, 0x34, 0xa9, 0xd7, 0x91, 0xe9, 0x14, 0x91, 0xeb, 0x37, 0x54, 0xd0, 0x37, 0x99, 0x79, 0x0f,
        0xe2, 0xd3, 0x08, 0xd1, 0x61, 0x46, 0xd5, 0xc9, 0xb0, 0xd0, 0xde, 0xbd, 0x97, 0xd7, 0x9c, 0xe8,
    };
    struct aws_byte_cursor message_input = aws_byte_cursor_from_array(message, sizeof(message));
    uint8_t hash[AWS_SHA256_LEN];
    AWS_ZERO_ARRAY(hash);
    struct aws_byte_buf hash_value = aws_byte_buf_from_empty_array(hash, sizeof(hash));
    aws_sha256_compute(allocator, &message_input, &hash_value, 0);

    struct aws_ecc_signer *signer = aws_ecc_signer_new(allocator, signing_key);
    size_t signature_size = aws_ecc_signer_max_signature_length(signer);

    struct aws_byte_buf signature_buf;
    AWS_ZERO_STRUCT(signature_buf);
    aws_byte_buf_init(&signature_buf, allocator, signature_size);

    struct aws_byte_cursor hash_cur = aws_byte_cursor_from_buf(&hash_value);
    ASSERT_SUCCESS(aws_ecc_signer_sign_hash(signer, &hash_cur, &signature_buf));

    struct aws_ecc_signer *verifying_signer = aws_ecc_signer_new(allocator, verifying_key);
    struct aws_byte_cursor signature_cur = aws_byte_cursor_from_buf(&signature_buf);
    ASSERT_SUCCESS(aws_ecc_signer_verify_signature(verifying_signer, &hash_cur, &signature_cur));

    aws_byte_buf_clean_up(&signature_buf);
    aws_ecc_signer_destroy(verifying_signer);
    aws_ecc_signer_destroy(signer);
    aws_ecc_key_pair_destroy(verifying_key);
    aws_ecc_key_pair_destroy(signing_key);

    return AWS_OP_SUCCESS;
}

AWS_TEST_CASE(ecdsa_p256_test_known_signing_value, s_ecdsa_p256_test_known_signing_value_fn)
