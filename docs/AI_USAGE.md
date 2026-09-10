### "Isn't this just AI slop?"

**No.**

The code is generated with generative AI, but the process is directed, constrained, reviewed, tested, and iterated on by me. The goal is not to dump arbitrary generated C/C++ slop into the repository and call it finished. The goal is to use AI as a decompilation tool while still validating the result against the original game.

If a function is wrong, does not behave correctly, does not compile, produces incorrect assembly, or otherwise fails the project's acceptance criteria, then it is not considered finished simply because an AI produced it.

This decompilation is still in its early stages. Code quality, naming, structure, and documentation will undoubtedly improve as a more substantial portion of the game's codebase is recovered and the relationships between its systems become clearer.

Early reconstructed code should therefore **not necessarily be treated as representative of the project's eventual quality**.

### "AI-generated code is unmaintainable."

AI-generated code *can* be unmaintainable. So can human-written code.

Maintainability is judged by the resulting code, not by the identity of whoever or whatever typed it. Generated code may be rewritten, simplified, renamed, reformatted, or regenerated when necessary.

If you find genuinely poor code, open an issue or submit an improvement. "AI wrote it" by itself is not a technical bug report.

### "You didn't really decompile it if AI wrote the source."

The reconstructed source code is still derived from the original binary and validated against it.

Decompilation is the process of recovering a higher-level representation from compiled machine code. This project uses generative AI as part of that reconstruction process. The fact that an LLM proposes source code does not change what binary the project is attempting to reproduce.

You are free to prefer manual, human-driven decompilation. This project does not.

### "Why not decompile everything manually?"

Because that would require substantially more time, and this project deliberately explores a different workflow.

Manual reverse engineering is still involved where needed: understanding functions, data structures, compiler behavior, calling conventions, control flow, game systems, and generated output. AI primarily accelerates the source-reconstruction step.

### "Doesn't AI hallucinate?"

Of course it does.

That is why generated output is not assumed to be correct.

LLMs can invent logic, misunderstand control flow, choose incorrect types, fabricate abstractions, and confidently produce plausible-looking crap. Generated code therefore has to be validated against objective evidence from the original program which is exactly what is done here.

Hallucination is a limitation of the tool, not a reason to pretend the tool cannot be useful.

### "How can you trust any of the generated code?"

By not trusting it blindly.

Where possible, correctness is established through things such as compilation, binary/assembly comparison, runtime testing via py-desmume, behavioral comparison, cross-references, data-layout verification, and manual inspection.

The exact validation method depends on the part of the game being reconstructed.

### "Is the AI doing all the reverse engineering for you?"

No.

The AI generates a significant amount of reconstructed source code, but it operates under human direction. Humans still decide what to investigate, provide context, inspect binaries and disassembly, identify incorrect assumptions, resolve ambiguous behavior, design the project structure, validate results, and determine when something is actually acceptable.

"AI-generated" does not mean "human-uninvolved."

### "Isn't using AI cheating?"

There is no competition here.

This is an engineering and reverse-engineering project, not an exam. The objective is to reconstruct the game accurately and produce a useful decompilation for mods, ports, fun projects etc.

If your personal goal is to practice manual decompilation, you should absolutely do that. It is simply not a constraint of this project.

### "Wouldn't a traditional decompilation be higher quality?"

Possibly in some areas, possibly not.

A carefully reconstructed manual implementation can be extremely clean and accurate. AI can also accelerate repetitive reconstruction work and sometimes identify plausible structures very quickly. Both approaches have failure modes.

The relevant question for this repository is whether the resulting code meets the project's technical standards.

### "Are you replacing contributors with AI?"

No contributor is owed repetitive reconstruction work.

Human contributions are still valuable for reverse engineering, documentation, tooling, naming, type recovery, debugging, matching, code review, testing, research, and correcting generated output.

What this project will not do is artificially avoid useful automation simply to preserve work that a tool can perform effectively.

### "What if I simply don't like generative AI?"

That is fine.

You do not have to use this project, contribute to it, endorse its methodology, or agree with the decision.

Technical criticism is welcome. Repeatedly dismissing the repository as "AI slop" solely because generative AI is involved is not. If the only argument you can offer is the provenance of the code rather than anything wrong with the code itself, that is an utterly useless critique.

### "Can I submit manually written code?"

Of course!

The project is AI-generated by default, not ideologically opposed to humans writing code.

### "Can I criticize the generated code?"

Absolutely.

Specific criticism is useful:

* incorrect behavior;
* bad types;
* inaccurate control flow;
* poor naming;
* unnecessary complexity;
* incorrect matching;
* unsafe assumptions;
* compiler inaccuracies;
* bad abstractions.

"This is bad because AI" is not useful criticism.

### "So what is the actual policy here?"

Judge the output.

If the code is wrong, demonstrate that it is wrong.

If the code is difficult to maintain, explain why.

If the methodology introduces a concrete problem, document it.

But if your entire objection is that generative AI was used at all, this is probably not the project for you. And if you find yourself dismissing technically validated work purely because of the tool used to produce it, you may want to examine that assumption before arrogantly lecturing others about engineering quality.
