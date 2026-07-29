# PgExtAssure enterprise pilot

This repository is a reproducible reference for signed PostgreSQL-extension
admission evidence. It demonstrates two intentionally different decisions:

- `extensions/approved`: a minimal SQL-only extension expected to pass;
- `extensions/rejected`: an intentionally unsafe extension expected to be
  blocked without losing its evidence.

The workflow pins PgExtAssure `v0.1.0-alpha.6` by immutable release commit SHA.
It creates an Evidence Bundle 1.0, Agent Review Pack 1.0, and offline-verified
Decision Ledger 1.0. It publishes GitHub OIDC/Sigstore attestations for:

1. the PgExtAssure admission predicate;
2. the SPDX 2.3 analyzed-source inventory;
3. the review pack, ledger, and offline verification result.

The rejected case is expected to make the scanner step fail. The workflow
retains and attests that blocked decision, validates it, and succeeds only when
the failure is the expected policy outcome.

## Reproduce

Run **Signed enterprise admission evidence** manually or push to `main`. Then
download either workflow artifact and verify it:

```bash
gh attestation verify evidence.zip \
  --repo borborich/pgextassure-enterprise-pilot \
  --signer-workflow \
  borborich/pgextassure-enterprise-pilot/.github/workflows/evidence.yml \
  --predicate-type \
  https://github.com/borborich/pgextassure/attestation/evidence/v1
```

Verify the separate SPDX attestation by replacing the predicate type with
`https://spdx.dev/Document/v2.3`.

After installing PgExtAssure `v0.1.0-alpha.6`, verify the internal bundle
contract without network access:

```bash
pgextassure evidence verify evidence.zip \
  --format json \
  --predicate-output verified-predicate.json \
  --sbom-output verified-sbom.spdx.json

pgextassure review verify \
  review.json \
  decisions.json
```

Cryptographic verification establishes provenance and integrity. PgExtAssure
verification establishes the internal evidence contract. Neither is a security
certificate or proof that an extension is safe.

The supplied ledger intentionally leaves every task `unresolved`. This proves
the pack/ledger correlation and offline verification path without pretending
that CI performed expert review. Both artifacts state
`can_grant_admission: false`.

## Trust boundaries

- Extension source is treated as untrusted data and is never executed.
- The policy and evidence output live outside the scanned extension root.
- Every third-party Action and PgExtAssure itself are pinned to full commits.
- Reports contain paths and matched evidence excerpts and should be handled as
  security artifacts.
- The SPDX document is an analyzed-source inventory, not a claim of dependency
  resolution or source-to-binary equivalence.

See the canonical
[PgExtAssure repository](https://github.com/borborich/pgextassure) for the
scanner, schemas, threat model, and enterprise-pilot documentation.

## Verified reference run

The initial [public workflow run](https://github.com/borborich/pgextassure-enterprise-pilot/actions/runs/30444207755)
was produced from pilot commit
`823065b0082514e371719ba21c2029d40023f1d6`.

| Case | Gate | Evidence Bundle SHA-256 |
| --- | --- | --- |
| approved | `pass` | `f32163888db53b2351bc6e549a7652df1af5e12b435f8dde589f399e0efa333b` |
| rejected | `blocked` | `0125fb52d9548a9e138bde6b6de014527ccfd601dc7a7bb06a645eb28563cfe2` |

Both bundles independently passed:

- PgExtAssure offline contract verification;
- the custom admission attestation;
- the SPDX 2.3 attestation.

The exact policy digest in both decisions is
`sha256:e81e9a9ae7420ac63ac2b4829f56c91ce4ca898b8dfa33fd77a07d8638b881ae`.

## Verified alpha6 review run

The [alpha6 workflow run](https://github.com/borborich/pgextassure-enterprise-pilot/actions/runs/30448603260)
was produced from pilot commit
`41b47375b78e2110d631b9a1ee195fed9a9aaa74`.

| Case | Gate | Tasks | Evidence SHA-256 | Review Pack SHA-256 |
| --- | --- | ---: | --- | --- |
| approved | `pass` | 0 | `72bf3789d819532a64dbd720f984f6736b60d3244a74e43bc8c8b2069951c021` | `2efc975874d6494c0d525b3537efea21c4a2d29d74cf28bc7983024f6d8be517` |
| rejected | `blocked` | 5 unresolved | `33225d0c6e8dd535e7b6c09583907f005c4bd31e45fd72ba0630f9960ed72fdf` | `63f36c9c365b196676ecf9aea1994cf983e9e51a984fcd0d6a254c674ac0b8dc` |

Both downloaded cases independently passed:

- Evidence Bundle 1.0 offline verification;
- Decision Ledger 1.0 offline verification;
- the custom admission attestation;
- provenance verification for `review.json`, `decisions.json`, and
  `review-verification.json`.

The alpha6 policy digest in both decisions is
`sha256:9eb2358be567ae40f426e048307584c39c8a8daef643e4bd0ad24812d1292b64`.
Both verification summaries report `can_grant_admission: false`.
