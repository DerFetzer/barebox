/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * sha1_base.h - core logic for SHA-1 implementations
 *
 * Copyright (C) 2015 Linaro Ltd <ard.biesheuvel@linaro.org>
 */

#ifndef _CRYPTO_SHA1_BASE_H
#define _CRYPTO_SHA1_BASE_H

#include <digest.h>
#include <crypto/sha.h>
#include <linux/string.h>

#include <asm/unaligned.h>

typedef void (sha1_block_fn)(struct sha1_state *sst, u8 const *src, int blocks);

static inline int sha1_base_init(struct digest *desc)
{
	struct sha1_state *sctx = digest_ctx(desc);

	*sctx = (struct sha1_state){
		.state = { SHA1_H0, SHA1_H1, SHA1_H2, SHA1_H3, SHA1_H4 },
	};

	return 0;
}

static inline int sha1_base_do_update(struct digest *desc,
				      const u8 *data,
				      unsigned int len,
				      sha1_block_fn *block_fn)
{
	struct sha1_state *sctx = digest_ctx(desc);
	unsigned int partial = sctx->count % SHA1_BLOCK_SIZE;

	sctx->count += len;

	if (unlikely((partial + len) >= SHA1_BLOCK_SIZE)) {
		int blocks;

		if (partial) {
			int p = SHA1_BLOCK_SIZE - partial;

			memcpy(sctx->buffer + partial, data, p);
			data += p;
			len -= p;

			block_fn(sctx, sctx->buffer, 1);
		}

		blocks = len / SHA1_BLOCK_SIZE;
		len %= SHA1_BLOCK_SIZE;

		if (blocks) {
			block_fn(sctx, data, blocks);
			data += blocks * SHA1_BLOCK_SIZE;
		}
		partial = 0;
	}
	if (len)
		memcpy(sctx->buffer + partial, data, len);

	return 0;
}

static inline int sha1_base_do_finalize(struct digest *desc,
					sha1_block_fn *block_fn)
{
	const int bit_offset = SHA1_BLOCK_SIZE - sizeof(__be64);
	struct sha1_state *sctx = digest_ctx(desc);
	__be64 *bits = (__be64 *)(sctx->buffer + bit_offset);
	unsigned int partial = sctx->count % SHA1_BLOCK_SIZE;

	sctx->buffer[partial++] = 0x80;
	if (partial > bit_offset) {
		memset(sctx->buffer + partial, 0x0, SHA1_BLOCK_SIZE - partial);
		partial = 0;

		block_fn(sctx, sctx->buffer, 1);
	}

	memset(sctx->buffer + partial, 0x0, bit_offset - partial);
	*bits = cpu_to_be64(sctx->count << 3);
	block_fn(sctx, sctx->buffer, 1);

	return 0;
}

static inline int sha1_base_finish(struct digest *desc, u8 *out)
{
	struct sha1_state *sctx = digest_ctx(desc);
	__be32 *digest = (__be32 *)out;
	int i;

	for (i = 0; i < SHA1_DIGEST_SIZE / sizeof(__be32); i++)
		put_unaligned_be32(sctx->state[i], digest++);

	memzero_explicit(sctx, sizeof(*sctx));
	return 0;
}

#endif /* _CRYPTO_SHA1_BASE_H */