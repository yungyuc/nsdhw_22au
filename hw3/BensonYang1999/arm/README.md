# Testing on oracle clould arm server

## Library
Due to arm architecture lack of MKL library, I use blas library instead.

## Specification
```
$ uname -a
Linux instance 5.15.0-1021-oracle #27-Ubuntu SMP Fri Oct 14 20:04:20 UTC 2022 aarch64 aarch64 aarch64 GNU/Linux

$ lscpu
Architecture:            aarch64
  CPU op-mode(s):        32-bit, 64-bit
  Byte Order:            Little Endian
CPU(s):                  4
  On-line CPU(s) list:   0-3
Vendor ID:               ARM
  Model name:            Neoverse-N1
    Model:               1
    Thread(s) per core:  1
    Core(s) per cluster: 4
    Socket(s):           -
    Cluster(s):          1
    Stepping:            r3p1
    BogoMIPS:            50.00
    Flags:               fp asimd evtstrm aes pmull sha1 sha2 crc32 atomics fphp asimdhp cpuid asimdrdm lrcpc dcpop asimddp ssbs
NUMA:
  NUMA node(s):          1
  NUMA node0 CPU(s):     0-3
Vulnerabilities:
  Itlb multihit:         Not affected
  L1tf:                  Not affected
  Mds:                   Not affected
  Meltdown:              Not affected
  Mmio stale data:       Not affected
  Retbleed:              Not affected
  Spec store bypass:     Mitigation; Speculative Store Bypass disabled via prctl
  Spectre v1:            Mitigation; __user pointer sanitization
  Spectre v2:            Mitigation; CSV2, BHB
  Srbds:                 Not affected
  Tsx async abort:       Not affected

```
