# Man proc

## proc - process information pseudo-filesystem

The proc filesystem is a pseudo-filesystem which provides an
interface to kernel data structures.  It is commonly mounted at
/proc.  Typically, it is mounted automatically by the system, but it
can also be mounted manually using a command such as:

`mount -t proc proc /proc` 

## /proc/[pid]/cmdline

              This read-only file holds the complete command line for the
              process, unless the process is a zombie.  In the latter case,
              there is nothing in this file: that is, a read on this file
              will return 0 characters.  The command-line arguments appear
              in this file as a set of strings separated by null bytes
              ('\0'), with a further null byte after the last string.

              If, after an execve(2), the process modifies its argv strings,
              those changes will show up here.  This is not the same thing
              as modifying the argv array.

              Furthermore, a process may change the memory location that
              this file refers via prctl(2) operations such as
              PR_SET_MM_ARG_START.

              Think of this file as the command line that the process wants
              you to see.


## /proc/[pid]/auxv (since 2.6.0)
              This contains the contents of the ELF interpreter information
              passed to the process at exec time.  The format is one
              unsigned long ID plus one unsigned long value for each entry.
              The last entry contains two zeros.  See also getauxval(3).

              Permission to access this file is governed by a ptrace access
              mode PTRACE_MODE_READ_FSCREDS check; see ptrace(2).

`LD_SHOW_AUXV=1 /bin/dd`

#### Output:

```
AT_SYSINFO_EHDR: 0x7ffd9bb67000
AT_HWCAP:        bfebfbff
AT_PAGESZ:       4096
AT_CLKTCK:       100
AT_PHDR:         0x563cbec7d040
AT_PHENT:        56
AT_PHNUM:        9
AT_BASE:         0x7f1e3024c000
AT_FLAGS:        0x0
AT_ENTRY:        0x563cbec80fc0
AT_UID:          1000
AT_EUID:         1000
AT_GID:          1000
AT_EGID:         1000
AT_SECURE:       0
AT_RANDOM:       0x7ffd9bb2b959
AT_HWCAP2:       0x0
AT_EXECFN:       /bin/dd
AT_PLATFORM:     x86_64
```


#### PR_SET_MM_EXE_FILE

Supersede the /proc/pid/exe symbolic link with a new
                     one pointing to a new executable file identified by the
                     file descriptor provided in arg3 argument.  The file
                     descriptor should be obtained with a regular open(2)
                     call.

                     To change the symbolic link, one needs to unmap all
                     existing executable memory areas, including those
                     created by the kernel itself (for example the kernel
                     usually creates at least one executable memory area for
                     the ELF .text section).


	ret |= prctl(PR_SET_MM, PR_SET_MM_ARG_START,   arg_start, 0, 0);