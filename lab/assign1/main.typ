#import "@preview/arkheion:0.1.0": arkheion, arkheion-appendices

#show: arkheion.with(
  title: [Assignment 1

  STM32 HAL库源码分析],
  authors: (
    (name: "仇琨元", email: "11913019@mail.sustech.edu.cn", affiliation: "",),
  ),
  abstract: lorem(55),
  date: datetime.today().display("[month repr:short] [day], [year]"),
)
#set cite(style: "institute-of-electrical-and-electronics-engineers")

#set text(
  font: ("HarmonyOS Sans", "HarmonyOS Sans SC"),
  lang: "zh",
)

#show link: it=>[
  #underline(text(rgb("#4466ff"))[#it.body], stroke:(dash:"dashed"))
]

#show raw: set text(
  font: ("Sarasa Fixed SC Nerd Font"),
)

= HAL库简介

HAL (Hardware Abstraction Layer) 库是由STMicroelectronics官方提供的STM32硬件抽象层，旨在简化基于STM32微控制器系列产品的应用程序开发。HAL库将各个寄存器和内存映射表抽象为一组库函数和便于记忆的宏定义，使得开发者能够脱离不同型号单片机的底层细节, 专注于应用程序的核心逻辑, 而不必将大量时间耗费在逐个记忆和配置寄存器的控制字上.

由于微控制器等嵌入式设备的片上资源较为紧张, 最终生成的机器代码需要尽可能压缩体积、提高执行效率, 因此HAL库的主要组成部分是宏定义和内联函数, 在预处理阶段便会被编译器展开到具体的寄存器和内存映射, 而不是在运行阶段再进行嵌套的函数调用, 这样的设计最大程度上避免了函数调用带来的额外开销.

HAL库具有下面几个主要特点:

- 简单易用: 通过统一的API访问硬件资源. 免除了查找手册的步骤.
- 可移植性: 在不同STM32系列之间具有良好的可移植性.
- 良好性能: 编译器优化后能够生成近似于基于寄存器开发的高效代码.


// Add bibliography and create Bibiliography section
#bibliography("bibliography.bib")

// Create appendix section
#show: arkheion-appendices

=

== Appendix section

#lorem(100)
