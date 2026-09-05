# Local editable data

Region directories below `data/` are generated from a user's own matching ROM
and are ignored by Git. For example, the European editable project is created
with:

```powershell
python .\tools\data_mod.py export `
  --version eur `
  --files-root .\extract\eur\files `
  --project-root .\data\eur
```

Do not force-add exported text, scripts, statistics, media, or other game data.
See [`docs/DATA_MODDING.md`](../docs/DATA_MODDING.md) for the editable schemas
and build workflow.
