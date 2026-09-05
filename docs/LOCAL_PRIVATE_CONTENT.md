# Local private content

This repository contains reverse-engineering source, metadata, documentation,
and original project tooling. It does not distribute the game, extracted game
content, or proprietary build tools.

The following paths are intentionally local and ignored:

- `extract/eur/` and `extract/baserom_PiT_eur.nds`: files extracted from a ROM
  supplied by the user;
- `data/eur/`: editable text, scripts, tables, and other data exported from that
  private extraction;
- `tools/mwccarm/`: a compatible compiler installation supplied by the user;
- `private/`: save states, research captures, datamines, backups, and other
  private reference material;
- `build/`, `asm/`, and root-level ROM outputs: generated artifacts.

The public-content audit rejects these paths and common ROM, asset, emulator,
IDA-database, and proprietary executable formats if they become tracked:

```powershell
python .\tools\check_public_content.py
```

Always export from a legally obtained ROM. Never bypass the ignore rules with
`git add -f`. A clean audit does not replace a human review of new files or
constitute legal advice.
