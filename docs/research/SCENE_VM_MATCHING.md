# Scene dispatcher matching

`SceneVm_DispatchCommand` at `0x02081730..0x02083B1C` compiles to the original
9,196-byte size with a **100% byte match** and is linked into the European
ROM build. The implementation is a C switch with a 16-instruction (64-byte)
inline-assembly block in opcode `0x04E` for the height update and its original
signed-height conversion schedule.
All four linked VM dispatchers now total 53,700 bytes.

## Recovered data flow

Opcode `0x04E` aligns one object's signed screen height to another object's,
with command argument 2 as the offset. Each screen height is the signed 16-bit
conversion of `render_height + 16 * (192 - y)`.

The current object contributes its original render height twice: once to its
wide screen projection and again to the final height update. The recovered C
candidate captures that value immediately after the second lookup and
projects the current object before the reference object. This matches the
original field reads, both wide projections, subtraction, offset addition and
store; only the conversion schedule differed.

The maintained assembly block covers dispatcher offsets `0x648..0x684`
inclusive, ending at `0x688`. It performs only that height calculation and
write, retaining the original height in `r0` across its two uses. The object
lookups, resource-flag copying, and all other opcode handlers remain C.

The C-only reconstruction reached 99.86951% instruction similarity. Its last
three differences were the conversion schedule below; the maintained inline
assembly emits the reference sequence exactly.

| Dispatcher offset | Reference and linked output | C-only reconstruction |
|---|---|---|
| `0x668` | `mov r2, r1, lsl #16` | `mov r1, r1, lsl #16` |
| `0x66C` | `mov r1, r3, lsl #16` | `mov r2, r1, asr #16` |
| `0x670` | `mov r2, r2, asr #16` | `mov r1, r3, lsl #16` |

## Compiler evidence

The configured compiler is `1.2/base`, internally version 2.0 build 72.
Its backend diagnostic function at executable VA `0x004E8520` contains only
`ret`; enabling `debuglisting` or `dumpir_postiro` cannot print those dumps.
Process-local debugger snapshots instead captured the live PCode graph through
instruction selection, coalescing, expansion, scheduling and coloring. The
instrumented and ordinary compiler runs produced identical object code.

The snapshots establish these distinctions:

- The earlier source's final compound assignment introduced the initial
  render-height read at a different point in the virtual-register sequence.
  An explicit, twice-used initial-height value fixes its allocation.
- Coalescing removes copies into named C locals. It does not coalesce the
  Y, projected-Y and wide-sum arithmetic nodes with one another. Their equal
  physical registers arise during coloring.
- Ordinary `s16` conversions enter PCode as `iEXTH`. Their left-shift
  temporaries are created during backend expansion. Explicit unsigned
  left-shift stages create those temporaries earlier instead.
- The C-only candidate expands the reference conversion before the current-object
  conversion. After the latter's arithmetic right shift is folded into the
  subtraction, `ASRother` and `LSLself` have equal remaining dependency lengths.
  The observed schedule picks `ASRother` first. The reference needs the opposite
  order, keeping both shifted values live simultaneously.

Changing variable names or declaration order does not preserve the discarded
copies. Inline scalar getters, a current-first narrowing helper, local short
pairs, scalar output-parameter helpers, and C++ value constructors normalized
to the same remaining sequence. Volatile locals added stack accesses; explicit
fixed-point division added a signed-division bias. None recovered the required
schedule. The 16-instruction inline-assembly block therefore preserves the
proven operation and exact original instructions within the C handler.
No source-permutation or compiler-option sweep was used.

Inline assembly also affects MWCC's optimization of the surrounding C. The
final source preserves those register schedules by capturing the object-ID
argument before packing the following arguments and writing motion-property
results directly through `VM_WriteVariable`. Relative movement forms explicit
`dx`/`dy`/`dz` target sums before their squared length; stopping all sound tasks
uses the original bottom-tested loop. Rewind helpers capture the script
pointer before decoding mode, result and argument counts in that order. A
separate command-pointer helper keeps the dynamic opcode read inside the
helper. These C forms keep the assembly confined to the height block.

## Symbolic verification

After building the two objects, run:

```sh
python -m pip install capstone pyelftools z3-solver
python tools/verify_scene_alignment.py --negative-control
```

The checker reads and decodes the actual reference and compiled object bytes
at dispatcher offsets `0x648..0x684`. It symbolically executes their integer
instructions, using unconstrained bit vectors for both objects' two 16-bit
fields and the 32-bit alignment offset. It compares all final register values,
the one 16-bit height write, and the absence of flag changes. Other instructions,
memory accesses, or writes outside this model fail validation.

Z3 returns `unsat`: no input distinguishes the two blocks under this model.
Independent field values also cover the case where both lookups return the
same object, by equality of the corresponding inputs. A negative control that
changes one shift from 16 to 15 returns `sat` and is rejected.
The negative control changes only a copy in memory, leaving both object files
untouched.

This proves the height block's machine-code semantics for the modeled inputs.
The separate objdiff comparison establishes the complete dispatcher's 9,196-byte
match, and the native link checks verify its integration. The assembly
fragment depends on the documented ARM instruction schedule; the proof does
not establish source-language portability or unrelated runtime behavior.

The linked build passes module and symbol checks with the maintained Scene
object included. The normal ROM build retains the reference SHA-1
`ba4ec2f99b4f2e0047601552bccf00aa73e28701`.
All 66 Python tests pass.
