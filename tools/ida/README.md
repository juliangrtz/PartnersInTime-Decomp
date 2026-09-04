# IDA overlay-2 database

The repository can generate an IDA 9.1/9.2 database for the European battle
overlay without committing ROM-derived bytes. First produce the verified raw
overlay from your own matching ROM:

```powershell
$env:Path = 'C:\Program Files\LLVM\bin;' + $env:Path
python tools\relink_native.py `
  --rom 'D:\NDS\Partners in Time\PiT.nds' --version eur `
  --output-rom build\PiT_eur_native_relinked.nds --require-matching
```

Then run IDA's text-mode executable from the repository root. IDA's raw-binary
`-b` argument uses paragraphs, so `2065D4` represents runtime address
`0x02065D40`:

```powershell
$ida = 'C:\Program Files\IDA Essential 9.2\idat.exe'
& $ida '-A' '-c' '-TBinary file' '-pARM' '-b2065D4' '-i2065D40' `
  '-Stools/ida/import_overlay2.py' '-Lbuild/ida/ov002_arm32.log' `
  '-obuild/ida/PiT_eur_ov002_battle.i64' `
  'build/reassembly/eur/native/arm9/arm9_ov002/arm9_ov002.bin'
```

`import_overlay2.py` corrects the raw loader's automatic AArch64 choice,
selects 32-bit ARMv5TE, imports the maintained DSD symbol map and exact function
bounds, and applies the currently reconstructed battle-AI types. The generated
database and log remain ignored build artifacts. Open the `.i64` file with the
GUI and press F5 on a named function.

For a quick batch decompiler check:

```powershell
& $ida '-A' '-Stools/ida/decompile_function.py BattleAI_HandleVmResult' `
  'build/ida/PiT_eur_ov002_battle.i64'
```

For an address-annotated instruction listing without opening the GUI, use the
companion disassembly helper:

```powershell
& $ida '-A' '-Stools/ida/disassemble_function.py BattleAI_HandleVmResult' `
  'build/ida/PiT_eur_ov002_battle.i64'
```

To identify the containing function and incoming/outgoing code references for
one or more runtime addresses:

```powershell
& $ida '-A' '-Stools/ida/inspect_address.py 0x02092918 0x02092A2C' `
  'build/ida/PiT_eur_ov002_battle.i64'
```

The resident ARM9 image can be imported in the same way. This database is the
useful companion for the generic script VM, save logic, item tables, and engine
code outside overlays:

```powershell
& $ida '-A' '-c' '-TBinary file' '-pARM' '-b200400' '-i02004000' `
  '-Stools/ida/import_arm9.py' '-Lbuild/ida/arm9.log' `
  '-obuild/ida/PiT_eur_arm9.i64' 'build/eur/build/arm9.bin'

& $ida '-A' '-Stools/ida/decompile_function.py VM_ExecuteCommand' `
  'build/ida/PiT_eur_arm9.i64'
```

`import_arm9.py` splits text, init, read-only data, constructors, data, and BSS;
imports every maintained resident symbol/function boundary; and applies the
reconstructed script-VM types before Hex-Rays runs.

IDA may print warnings from optional third-party plugins whose Python packages
are absent. They do not affect the ARM processor module, the database import,
or Hex-Rays.
