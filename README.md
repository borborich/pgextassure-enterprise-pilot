# PgExtAssure enterprise pilot

This repository is a reproducible reference for signed PostgreSQL-extension
admission evidence. It demonstrates two intentionally different decisions:

- `extensions/approved`: a minimal SQL-only extension expected to pass;
- `extensions/rejected`: an intentionally unsafe extension expected to be
  blocked without losing its evidence.

The workflow pins PgExtAssure `v0.1.0-alpha.5` by immutable release commit SHA.
It creates an Evidence Bundle 1.0, verifies it offline, and publishes two
GitHub OIDC/Sigstore attestations for each bundle:

1. the PgExtAssure admission predicate;
2. the SPDX 2.3 analyzed-source inventory.

The rejected case is expected to make the scanner step fail. The workflow
retains and attests that blocked decision, validates it, and succeeds only when
the failure is the expected policy outcome.

## Reproduce

Run **Signed enterprise admission evidence** manually or push to `main`. Then
download either workflow artifact and verify it:

```bash
gh attestation verify evidence.zip \
  --repo borborich/pgextassure-enterprise-pilot \
  --predicate-type \
  https://github.com/borborich/pgextassure/attestation/evidence/v1
```

After installing PgExtAssure `v0.1.0-alpha.5`, verify the internal bundle
contract without network access:

```bash
pgextassure evidence verify evidence.zip \
  --format json \
  --predicate-output verified-predicate.json \
  --sbom-output verified-sbom.spdx.json
```

Cryptographic verification establishes provenance and integrity. PgExtAssure
verification establishes the internal evidence contract. Neither is a security
certificate or proof that an extension is safe.

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
