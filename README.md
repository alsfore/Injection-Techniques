# Injection Techniques - Fore <3

```
// present day. present time. hahahaha.

```

* * *

```
┌─────────────────────────────────────────────────────────────────┐
│  PROTOCOL 06 // INJECTION FRAMEWORK                             │
│  "the signal must flow. the body is just hardware."             │
└─────────────────────────────────────────────────────────────────┘
```

&gt; _"memories change shape as they move from one vessel to another."_

a collection of **process injection techniques** for windows environments.  
from classic to modern. from userland to the edge of the kernel.

this repository documents the methods used to introduce foreign code  
into running processes — for red team operations, malware analysis,  
and windows internals research.

* * *

```
┌─────────────────────────────────────────────────────────────────┐
│  SIGNAL STATUS // CURRENT IMPLEMENTATION                        │
│  uploading to the wired...                                      │
└─────────────────────────────────────────────────────────────────┘
```

```
[ ACTIVE SIGNALS ]
├─ dll-injection.c          :: classic remote thread + loadlibrary
│   └─ standard reflective loader precursor
│
└─ shellcode-injection.c    :: direct memory write + execution
    ├─ virtualallocex -&gt; writeprocessmemory -&gt; createremotethread
    └─ local process injection variants

```

* * *

```
┌─────────────────────────────────────────────────────────────────┐
│  PENDING TRANSMISSIONS // ROADMAP                               │
│  "the wired was always meant to grow."                          │
└─────────────────────────────────────────────────────────────────┘
```

```
[ UPCOMING PROTOCOLS ]
□ direct_syscalls/         :: bypass ntdll hooks, direct kernel entry
□ apc_injection/           :: queueuserapc & ntqueueapcthread
□ earlybird_apc/           :: pre-execution apc queuing
□ thread_hijacking/        :: suspend, inject, resume, redirect
□ reflective_dll/          :: self-mapping portable executables
□ process_doppelganging/   :: transacted file operations + rollback
□ process_ghosting/        :: delete-pending file mapping trick

```

* * *

```
┌─────────────────────────────────────────────────────────────────┐
│  USAGE // RUNNING THE SIGNAL                                    │
│  "i need to make sure you're who you say you are."              │
└─────────────────────────────────────────────────────────────────┘
```

* * *

```
┌─────────────────────────────────────────────────────────────────┐
│  DISCLAIMER // LEGAL BOUNDARY                                   │
│  "if you're not remembered, you never existed."                 │
└─────────────────────────────────────────────────────────────────┘
```

&gt; **WARNING:** These techniques are provided for **authorized security research**,  
&gt; **maldev reference**, and **malware analysis** only.  

&gt; Unauthorized access to computer systems is illegal.  
&gt; The author assumes no liability for misuse.  
&gt; Know the law. Know your scope. Stay in the legal world.

* * *

```
┌─────────────────────────────────────────────────────────────────┐
│  ATTRIBUTION // SIGNAL SOURCES                                  │
│  "we are all connected. everything is connected."               │
└─────────────────────────────────────────────────────────────────┘
```

- [Reflective DLL Injection - Stephen Fewer](https://github.com/stephenfewer/ReflectiveDLLInjection)
- [Process Doppelgänging - Tal Liberman & Eugene Kogan](https://www.blackhat.com/docs/eu-17/materials/eu-17-Liberman-Behind-The-Scenes-Of-Attackers-Mind-Process-Doppelganging.pdf)
- [Process Ghosting - Gabriel Landau](https://www.elastic.co/blog/process-ghosting-a-new-executable-image-tampering-attack)
- [Direct Syscalls - j00ru](https://j00ru.vexillium.org/syscalls/nt/64/)

* * *

```
┌─────────────────────────────────────────────────────────────────┐
│  LAYER 7 // CONTACT                                             │
│  "the border between the real world and the wired is fading."   │
└─────────────────────────────────────────────────────────────────┘
```

for questions/corrections:

**blog:** [whatsafed.xyz](https://whatsafed.xyz)  
**github:** [@alsfore](https://github.com/alsfore)

* * *

---
**License:** MIT - See [LICENSE](LICENSE) for details.  
