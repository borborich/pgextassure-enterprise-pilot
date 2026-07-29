# PgExtAssure enterprise pilot

This repository is a reproducible reference for signed PostgreSQL-extension
admission evidence. It demonstrates two intentionally different decisions:

- `extensions/approved`: a minimal SQL-only extension expected to pass;
- `extensions/rejected`: an intentionally unsafe extension expected to be
  blocked without losing its evidence.

The workflow pins PgExtAssure `v0.1.0-alpha.7` by immutable release commit SHA
and supplies a reviewed Scope Plan 1.0. It creates an Evidence Bundle 1.0,
Agent Review Pack 1.0, and offline-verified Decision Ledger 1.0. It publishes
GitHub OIDC/Sigstore attestations for:

1. the PgExtAssure admission predicate;
2. the SPDX 2.3 analyzed-source inventory;
3. the review pack, ledger, and offline verification result.

The rejected case is expected to make the scanner step fail. The workflow
retains and attests that blocked decision, validates it, and succeeds only when
the failure is the expected policy outcome.

The reference scope plan deliberately selects the complete extension root with
no exclusions. This exercises scope provenance and offline correlation without
hiding any pilot input. A production adopter should add an exclusion only
after reviewing and pinning the exact file bytes or symlink-target text.

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

After installing PgExtAssure `v0.1.0-alpha.7`, verify the internal bundle
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
- The exact Scope Plan bytes are embedded in each Evidence Bundle and
  correlated with report metadata by offline verification.
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

## Verified alpha7 scope-bound run

The [alpha7 workflow run](https://github.com/borborich/pgextassure-enterprise-pilot/actions/runs/30453940591)
was produced from pilot commit
`553661d7d96584ddb5fd8e032989bfe5561407c6` and immutable PgExtAssure
release commit `ed6c354e5c629d9c0ede3d617524fbc8acf76c18`.

| Case | Gate | Tasks | Evidence SHA-256 | Review Pack SHA-256 | Decision Ledger SHA-256 |
| --- | --- | ---: | --- | --- | --- |
| approved | `pass` | 0 | `622abacf32e5aa259f88f60eec60bc417214673ce4a5b17c0544b984ea5fdd3e` | `f672ec02926c8cebb7bb298d3df9c375e26c7eeb79d094ffe88503be82f10bba` | `61cb965f5d9a26aa8356bbdfe5ec86ddfc2520cd0178a3fff0edad5c30795e88` |
| rejected | `blocked` | 5 unresolved | `70c26e6e3b380a8d9e287b83468dce1a9a9edb058b9aede599476dfab4be7d52` | `714ed72bb21b807ef132ee5b8d96579058518ccb01d1a8ce4d1fe2e340430772` | `be9c48ee4e7621e246d913070f11e44ae9ec31d0c420d66b2ac26485d3f1ed5d` |

Both downloaded cases independently passed:

- Evidence Bundle 1.0 offline verification;
- exact Scope Plan 1.0 byte and metadata correlation;
- Decision Ledger 1.0 offline verification;
- the custom admission and SPDX attestations;
- provenance verification for the review pack, ledger, and verification
  summary.

The exact scope-plan digest in both reports is
`sha256:b60b25ccf9ab760e1fb0be1d56ba5e3089edffaee60761c710a7b56717b23321`.
The exact policy digest is
`sha256:c746a68e0203dfff539d745c57967e1b8fef24b6a4c70c5ab5072095beb327b5`.
Both verification summaries report `can_grant_admission: false`.
