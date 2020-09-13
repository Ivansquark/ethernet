#ifndef R2R_H_
#define R2R_H_


// need to set in r0:
//__asm volatile ("ldr r0, =0x40021018" "\n\t"); //загрузим в r0 адрес GPIOE->BSRR (32 бита)
//ftd0;__asm volatile ("str r1, [r0]" "\n\t"); // записываем значение r1 по адресу [r0]

#define ftd0 __asm volatile ("movt r1, #0xff00" "\n\t");
#define ftd1 __asm volatile ("mov r1, #0x0100" "\n\t" "movt r1, #0xff00" "\n\t");
#define ftd2 __asm volatile ("mov r1, #0x0200" "\n\t" "movt r1, #0xff00" "\n\t");
#define ftd3 __asm volatile ("mov r1, #0x0300" "\n\t" "movt r1, #0xff00" "\n\t");
#define ftd4 __asm volatile ("mov r1, #0x0400" "\n\t" "movt r1, #0xff00" "\n\t");
#define ftd5 __asm volatile ("mov r1, #0x0500" "\n\t" "movt r1, #0xff00" "\n\t");
#define ftd6 __asm volatile ("mov r1, #0x0600" "\n\t" "movt r1, #0xff00" "\n\t");
#define ftd7 __asm volatile ("mov r1, #0x0700" "\n\t" "movt r1, #0xff00" "\n\t");
#define ftd8 __asm volatile ("mov r1, #0x0800" "\n\t" "movt r1, #0xff00" "\n\t");
#define ftd9 __asm volatile ("mov r1, #0x0900" "\n\t" "movt r1, #0xff00" "\n\t");
#define ftd10 __asm volatile ("mov r1, #0x0a00" "\n\t" "movt r1, #0xff00" "\n\t");
#define ftd11 __asm volatile ("mov r1, #0x0b00" "\n\t" "movt r1, #0xff00" "\n\t");
#define ftd12 __asm volatile ("mov r1, #0x0c00" "\n\t" "movt r1, #0xff00" "\n\t");
#define ftd13 __asm volatile ("mov r1, #0x0d00" "\n\t" "movt r1, #0xff00" "\n\t");
#define ftd14 __asm volatile ("mov r1, #0x0e00" "\n\t" "movt r1, #0xff00" "\n\t");
#define ftd15 __asm volatile ("mov r1, #0x0f00" "\n\t" "movt r1, #0xff00" "\n\t");

#define ftd16 __asm volatile ("mov r1, #0x1000" "\n\t" "movt r1, #0xff00" "\n\t");
#define ftd17 __asm volatile ("mov r1, #0x1100" "\n\t" "movt r1, #0xff00" "\n\t");
#define ftd18 __asm volatile ("mov r1, #0x1200" "\n\t" "movt r1, #0xff00" "\n\t");
#define ftd19 __asm volatile ("mov r1, #0x1300" "\n\t" "movt r1, #0xff00" "\n\t");
#define ftd20 __asm volatile ("mov r1, #0x1400" "\n\t" "movt r1, #0xff00" "\n\t");
#define ftd21 __asm volatile ("mov r1, #0x1500" "\n\t" "movt r1, #0xff00" "\n\t");
#define ftd22 __asm volatile ("mov r1, #0x1600" "\n\t" "movt r1, #0xff00" "\n\t");
#define ftd23 __asm volatile ("mov r1, #0x1700" "\n\t" "movt r1, #0xff00" "\n\t");
#define ftd24 __asm volatile ("mov r1, #0x1800" "\n\t" "movt r1, #0xff00" "\n\t");
#define ftd25 __asm volatile ("mov r1, #0x1900" "\n\t" "movt r1, #0xff00" "\n\t");
#define ftd26 __asm volatile ("mov r1, #0x1a00" "\n\t" "movt r1, #0xff00" "\n\t");
#define ftd27 __asm volatile ("mov r1, #0x1b00" "\n\t" "movt r1, #0xff00" "\n\t");
#define ftd28 __asm volatile ("mov r1, #0x1c00" "\n\t" "movt r1, #0xff00" "\n\t");
#define ftd29 __asm volatile ("mov r1, #0x1d00" "\n\t" "movt r1, #0xff00" "\n\t");
#define ftd30 __asm volatile ("mov r1, #0x1e00" "\n\t" "movt r1, #0xff00" "\n\t");
#define ftd31 __asm volatile ("mov r1, #0x1f00" "\n\t" "movt r1, #0xff00" "\n\t");

#define ftd32 __asm volatile ("mov r1, #0x2000" "\n\t" "movt r1, #0xff00" "\n\t");
#define ftd33 __asm volatile ("mov r1, #0x2100" "\n\t" "movt r1, #0xff00" "\n\t");
#define ftd34 __asm volatile ("mov r1, #0x2200" "\n\t" "movt r1, #0xff00" "\n\t");
#define ftd35 __asm volatile ("mov r1, #0x2300" "\n\t" "movt r1, #0xff00" "\n\t");
#define ftd36 __asm volatile ("mov r1, #0x2400" "\n\t" "movt r1, #0xff00" "\n\t");
#define ftd37 __asm volatile ("mov r1, #0x2500" "\n\t" "movt r1, #0xff00" "\n\t");
#define ftd38 __asm volatile ("mov r1, #0x2600" "\n\t" "movt r1, #0xff00" "\n\t");
#define ftd39 __asm volatile ("mov r1, #0x2700" "\n\t" "movt r1, #0xff00" "\n\t");
#define ftd40 __asm volatile ("mov r1, #0x2800" "\n\t" "movt r1, #0xff00" "\n\t");
#define ftd41 __asm volatile ("mov r1, #0x2900" "\n\t" "movt r1, #0xff00" "\n\t");
#define ftd42 __asm volatile ("mov r1, #0x2a00" "\n\t" "movt r1, #0xff00" "\n\t");
#define ftd43 __asm volatile ("mov r1, #0x2b00" "\n\t" "movt r1, #0xff00" "\n\t");
#define ftd44 __asm volatile ("mov r1, #0x2c00" "\n\t" "movt r1, #0xff00" "\n\t");
#define ftd45 __asm volatile ("mov r1, #0x2d00" "\n\t" "movt r1, #0xff00" "\n\t");
#define ftd46 __asm volatile ("mov r1, #0x2e00" "\n\t" "movt r1, #0xff00" "\n\t");
#define ftd47 __asm volatile ("mov r1, #0x2f00" "\n\t" "movt r1, #0xff00" "\n\t");

#define ftd48 __asm volatile ("mov r1, #0x3000" "\n\t" "movt r1, #0xff00" "\n\t");
#define ftd49 __asm volatile ("mov r1, #0x3100" "\n\t" "movt r1, #0xff00" "\n\t");
#define ftd50 __asm volatile ("mov r1, #0x3200" "\n\t" "movt r1, #0xff00" "\n\t");
#define ftd51 __asm volatile ("mov r1, #0x3300" "\n\t" "movt r1, #0xff00" "\n\t");
#define ftd52 __asm volatile ("mov r1, #0x3400" "\n\t" "movt r1, #0xff00" "\n\t");
#define ftd53 __asm volatile ("mov r1, #0x3500" "\n\t" "movt r1, #0xff00" "\n\t");
#define ftd54 __asm volatile ("mov r1, #0x3600" "\n\t" "movt r1, #0xff00" "\n\t");
#define ftd55 __asm volatile ("mov r1, #0x3700" "\n\t" "movt r1, #0xff00" "\n\t");
#define ftd56 __asm volatile ("mov r1, #0x3800" "\n\t" "movt r1, #0xff00" "\n\t");
#define ftd57 __asm volatile ("mov r1, #0x3900" "\n\t" "movt r1, #0xff00" "\n\t");
#define ftd58 __asm volatile ("mov r1, #0x3a00" "\n\t" "movt r1, #0xff00" "\n\t");
#define ftd59 __asm volatile ("mov r1, #0x3b00" "\n\t" "movt r1, #0xff00" "\n\t");
#define ftd60 __asm volatile ("mov r1, #0x3c00" "\n\t" "movt r1, #0xff00" "\n\t");
#define ftd61 __asm volatile ("mov r1, #0x3d00" "\n\t" "movt r1, #0xff00" "\n\t");
#define ftd62 __asm volatile ("mov r1, #0x3e00" "\n\t" "movt r1, #0xff00" "\n\t");
#define ftd63 __asm volatile ("mov r1, #0x3f00" "\n\t" "movt r1, #0xff00" "\n\t");

#define ftd64 __asm volatile ("mov r1, #0x4000" "\n\t" "movt r1, #0xff00" "\n\t");
#define ftd65 __asm volatile ("mov r1, #0x4100" "\n\t" "movt r1, #0xff00" "\n\t");
#define ftd66 __asm volatile ("mov r1, #0x4200" "\n\t" "movt r1, #0xff00" "\n\t");
#define ftd67 __asm volatile ("mov r1, #0x4300" "\n\t" "movt r1, #0xff00" "\n\t");
#define ftd68 __asm volatile ("mov r1, #0x4400" "\n\t" "movt r1, #0xff00" "\n\t");
#define ftd69 __asm volatile ("mov r1, #0x4500" "\n\t" "movt r1, #0xff00" "\n\t");
#define ftd70 __asm volatile ("mov r1, #0x4600" "\n\t" "movt r1, #0xff00" "\n\t");
#define ftd71 __asm volatile ("mov r1, #0x4700" "\n\t" "movt r1, #0xff00" "\n\t");
#define ftd72 __asm volatile ("mov r1, #0x4800" "\n\t" "movt r1, #0xff00" "\n\t");
#define ftd73 __asm volatile ("mov r1, #0x4900" "\n\t" "movt r1, #0xff00" "\n\t");
#define ftd74 __asm volatile ("mov r1, #0x4a00" "\n\t" "movt r1, #0xff00" "\n\t");
#define ftd75 __asm volatile ("mov r1, #0x4b00" "\n\t" "movt r1, #0xff00" "\n\t");
#define ftd76 __asm volatile ("mov r1, #0x4c00" "\n\t" "movt r1, #0xff00" "\n\t");
#define ftd77 __asm volatile ("mov r1, #0x4d00" "\n\t" "movt r1, #0xff00" "\n\t");
#define ftd78 __asm volatile ("mov r1, #0x4e00" "\n\t" "movt r1, #0xff00" "\n\t");
#define ftd79 __asm volatile ("mov r1, #0x4f00" "\n\t" "movt r1, #0xff00" "\n\t");

#define ftd80 __asm volatile ("mov r1, #0x5000" "\n\t" "movt r1, #0xff00" "\n\t");
#define ftd81 __asm volatile ("mov r1, #0x5100" "\n\t" "movt r1, #0xff00" "\n\t");
#define ftd82 __asm volatile ("mov r1, #0x5200" "\n\t" "movt r1, #0xff00" "\n\t");
#define ftd83 __asm volatile ("mov r1, #0x5300" "\n\t" "movt r1, #0xff00" "\n\t");
#define ftd84 __asm volatile ("mov r1, #0x5400" "\n\t" "movt r1, #0xff00" "\n\t");
#define ftd85 __asm volatile ("mov r1, #0x5500" "\n\t" "movt r1, #0xff00" "\n\t");
#define ftd86 __asm volatile ("mov r1, #0x5600" "\n\t" "movt r1, #0xff00" "\n\t");
#define ftd87 __asm volatile ("mov r1, #0x5700" "\n\t" "movt r1, #0xff00" "\n\t");
#define ftd88 __asm volatile ("mov r1, #0x5800" "\n\t" "movt r1, #0xff00" "\n\t");
#define ftd89 __asm volatile ("mov r1, #0x5900" "\n\t" "movt r1, #0xff00" "\n\t");
#define ftd90 __asm volatile ("mov r1, #0x5a00" "\n\t" "movt r1, #0xff00" "\n\t");
#define ftd91 __asm volatile ("mov r1, #0x5b00" "\n\t" "movt r1, #0xff00" "\n\t");
#define ftd92 __asm volatile ("mov r1, #0x5c00" "\n\t" "movt r1, #0xff00" "\n\t");
#define ftd93 __asm volatile ("mov r1, #0x5d00" "\n\t" "movt r1, #0xff00" "\n\t");
#define ftd94 __asm volatile ("mov r1, #0x5e00" "\n\t" "movt r1, #0xff00" "\n\t");
#define ftd95 __asm volatile ("mov r1, #0x5f00" "\n\t" "movt r1, #0xff00" "\n\t");

#define ftd96 __asm volatile ("mov r1, #0x6000" "\n\t" "movt r1, #0xff00" "\n\t");
#define ftd97 __asm volatile ("mov r1, #0x6100" "\n\t" "movt r1, #0xff00" "\n\t");
#define ftd98 __asm volatile ("mov r1, #0x6200" "\n\t" "movt r1, #0xff00" "\n\t");
#define ftd99 __asm volatile ("mov r1, #0x6300" "\n\t" "movt r1, #0xff00" "\n\t");
#define ftd100 __asm volatile ("mov r1, #0x6400" "\n\t" "movt r1, #0xff00" "\n\t");
#define ftd101 __asm volatile ("mov r1, #0x6500" "\n\t" "movt r1, #0xff00" "\n\t");
#define ftd102 __asm volatile ("mov r1, #0x6600" "\n\t" "movt r1, #0xff00" "\n\t");
#define ftd103 __asm volatile ("mov r1, #0x6700" "\n\t" "movt r1, #0xff00" "\n\t");
#define ftd104 __asm volatile ("mov r1, #0x6800" "\n\t" "movt r1, #0xff00" "\n\t");
#define ftd105 __asm volatile ("mov r1, #0x6900" "\n\t" "movt r1, #0xff00" "\n\t");
#define ftd106 __asm volatile ("mov r1, #0x6a00" "\n\t" "movt r1, #0xff00" "\n\t");
#define ftd107 __asm volatile ("mov r1, #0x6b00" "\n\t" "movt r1, #0xff00" "\n\t");
#define ftd108 __asm volatile ("mov r1, #0x6c00" "\n\t" "movt r1, #0xff00" "\n\t");
#define ftd109 __asm volatile ("mov r1, #0x6d00" "\n\t" "movt r1, #0xff00" "\n\t");
#define ftd110 __asm volatile ("mov r1, #0x6e00" "\n\t" "movt r1, #0xff00" "\n\t");
#define ftd111 __asm volatile ("mov r1, #0x6f00" "\n\t" "movt r1, #0xff00" "\n\t");

#define ftd112 __asm volatile ("mov r1, #0x7000" "\n\t" "movt r1, #0xff00" "\n\t");
#define ftd113 __asm volatile ("mov r1, #0x7100" "\n\t" "movt r1, #0xff00" "\n\t");
#define ftd114 __asm volatile ("mov r1, #0x7200" "\n\t" "movt r1, #0xff00" "\n\t");
#define ftd115 __asm volatile ("mov r1, #0x7300" "\n\t" "movt r1, #0xff00" "\n\t");
#define ftd116 __asm volatile ("mov r1, #0x7400" "\n\t" "movt r1, #0xff00" "\n\t");
#define ftd117 __asm volatile ("mov r1, #0x7500" "\n\t" "movt r1, #0xff00" "\n\t");
#define ftd118 __asm volatile ("mov r1, #0x7600" "\n\t" "movt r1, #0xff00" "\n\t");
#define ftd119 __asm volatile ("mov r1, #0x7700" "\n\t" "movt r1, #0xff00" "\n\t");
#define ftd120 __asm volatile ("mov r1, #0x7800" "\n\t" "movt r1, #0xff00" "\n\t");
#define ftd121 __asm volatile ("mov r1, #0x7900" "\n\t" "movt r1, #0xff00" "\n\t");
#define ftd122 __asm volatile ("mov r1, #0x7a00" "\n\t" "movt r1, #0xff00" "\n\t");
#define ftd123 __asm volatile ("mov r1, #0x7b00" "\n\t" "movt r1, #0xff00" "\n\t");
#define ftd124 __asm volatile ("mov r1, #0x7c00" "\n\t" "movt r1, #0xff00" "\n\t");
#define ftd125 __asm volatile ("mov r1, #0x7d00" "\n\t" "movt r1, #0xff00" "\n\t");
#define ftd126 __asm volatile ("mov r1, #0x7e00" "\n\t" "movt r1, #0xff00" "\n\t");
#define ftd127 __asm volatile ("mov r1, #0x7f00" "\n\t" "movt r1, #0xff00" "\n\t");

#define ftd128 __asm volatile ("mov r1, #0x8000" "\n\t" "movt r1, #0xff00" "\n\t");
#define ftd129 __asm volatile ("mov r1, #0x8100" "\n\t" "movt r1, #0xff00" "\n\t");
#define ftd130 __asm volatile ("mov r1, #0x8200" "\n\t" "movt r1, #0xff00" "\n\t");
#define ftd131 __asm volatile ("mov r1, #0x8300" "\n\t" "movt r1, #0xff00" "\n\t");
#define ftd132 __asm volatile ("mov r1, #0x8400" "\n\t" "movt r1, #0xff00" "\n\t");
#define ftd133 __asm volatile ("mov r1, #0x8500" "\n\t" "movt r1, #0xff00" "\n\t");
#define ftd134 __asm volatile ("mov r1, #0x8600" "\n\t" "movt r1, #0xff00" "\n\t");
#define ftd135 __asm volatile ("mov r1, #0x8700" "\n\t" "movt r1, #0xff00" "\n\t");
#define ftd136 __asm volatile ("mov r1, #0x8800" "\n\t" "movt r1, #0xff00" "\n\t");
#define ftd137 __asm volatile ("mov r1, #0x8900" "\n\t" "movt r1, #0xff00" "\n\t");
#define ftd138 __asm volatile ("mov r1, #0x8a00" "\n\t" "movt r1, #0xff00" "\n\t");
#define ftd139 __asm volatile ("mov r1, #0x8b00" "\n\t" "movt r1, #0xff00" "\n\t");
#define ftd140 __asm volatile ("mov r1, #0x8c00" "\n\t" "movt r1, #0xff00" "\n\t");
#define ftd141 __asm volatile ("mov r1, #0x8d00" "\n\t" "movt r1, #0xff00" "\n\t");
#define ftd142 __asm volatile ("mov r1, #0x8e00" "\n\t" "movt r1, #0xff00" "\n\t");
#define ftd143 __asm volatile ("mov r1, #0x8f00" "\n\t" "movt r1, #0xff00" "\n\t");

#define ftd144 __asm volatile ("mov r1, #0x9000" "\n\t" "movt r1, #0xff00" "\n\t");
#define ftd145 __asm volatile ("mov r1, #0x9100" "\n\t" "movt r1, #0xff00" "\n\t");
#define ftd146 __asm volatile ("mov r1, #0x9200" "\n\t" "movt r1, #0xff00" "\n\t");
#define ftd147 __asm volatile ("mov r1, #0x9300" "\n\t" "movt r1, #0xff00" "\n\t");
#define ftd148 __asm volatile ("mov r1, #0x9400" "\n\t" "movt r1, #0xff00" "\n\t");
#define ftd149 __asm volatile ("mov r1, #0x9500" "\n\t" "movt r1, #0xff00" "\n\t");
#define ftd150 __asm volatile ("mov r1, #0x9600" "\n\t" "movt r1, #0xff00" "\n\t");
#define ftd151 __asm volatile ("mov r1, #0x9700" "\n\t" "movt r1, #0xff00" "\n\t");
#define ftd152 __asm volatile ("mov r1, #0x9800" "\n\t" "movt r1, #0xff00" "\n\t");
#define ftd153 __asm volatile ("mov r1, #0x9900" "\n\t" "movt r1, #0xff00" "\n\t");
#define ftd154 __asm volatile ("mov r1, #0x9a00" "\n\t" "movt r1, #0xff00" "\n\t");
#define ftd155 __asm volatile ("mov r1, #0x9b00" "\n\t" "movt r1, #0xff00" "\n\t");
#define ftd156 __asm volatile ("mov r1, #0x9c00" "\n\t" "movt r1, #0xff00" "\n\t");
#define ftd157 __asm volatile ("mov r1, #0x9d00" "\n\t" "movt r1, #0xff00" "\n\t");
#define ftd158 __asm volatile ("mov r1, #0x9e00" "\n\t" "movt r1, #0xff00" "\n\t");
#define ftd159 __asm volatile ("mov r1, #0x9f00" "\n\t" "movt r1, #0xff00" "\n\t");

#define ftd160 __asm volatile ("mov r1, #0xa000" "\n\t" "movt r1, #0xff00" "\n\t");
#define ftd161 __asm volatile ("mov r1, #0xa100" "\n\t" "movt r1, #0xff00" "\n\t");
#define ftd162 __asm volatile ("mov r1, #0xa200" "\n\t" "movt r1, #0xff00" "\n\t");
#define ftd163 __asm volatile ("mov r1, #0xa300" "\n\t" "movt r1, #0xff00" "\n\t");
#define ftd164 __asm volatile ("mov r1, #0xa400" "\n\t" "movt r1, #0xff00" "\n\t");
#define ftd165 __asm volatile ("mov r1, #0xa500" "\n\t" "movt r1, #0xff00" "\n\t");
#define ftd166 __asm volatile ("mov r1, #0xa600" "\n\t" "movt r1, #0xff00" "\n\t");
#define ftd167 __asm volatile ("mov r1, #0xa700" "\n\t" "movt r1, #0xff00" "\n\t");
#define ftd168 __asm volatile ("mov r1, #0xa800" "\n\t" "movt r1, #0xff00" "\n\t");
#define ftd169 __asm volatile ("mov r1, #0xa900" "\n\t" "movt r1, #0xff00" "\n\t");
#define ftd170 __asm volatile ("mov r1, #0xaa00" "\n\t" "movt r1, #0xff00" "\n\t");
#define ftd171 __asm volatile ("mov r1, #0xab00" "\n\t" "movt r1, #0xff00" "\n\t");
#define ftd172 __asm volatile ("mov r1, #0xac00" "\n\t" "movt r1, #0xff00" "\n\t");
#define ftd173 __asm volatile ("mov r1, #0xad00" "\n\t" "movt r1, #0xff00" "\n\t");
#define ftd174 __asm volatile ("mov r1, #0xae00" "\n\t" "movt r1, #0xff00" "\n\t");
#define ftd175 __asm volatile ("mov r1, #0xaf00" "\n\t" "movt r1, #0xff00" "\n\t");

#define ftd176 __asm volatile ("mov r1, #0xb000" "\n\t" "movt r1, #0xff00" "\n\t");
#define ftd177 __asm volatile ("mov r1, #0xb100" "\n\t" "movt r1, #0xff00" "\n\t");
#define ftd178 __asm volatile ("mov r1, #0xb200" "\n\t" "movt r1, #0xff00" "\n\t");
#define ftd179 __asm volatile ("mov r1, #0xb300" "\n\t" "movt r1, #0xff00" "\n\t");
#define ftd180 __asm volatile ("mov r1, #0xb400" "\n\t" "movt r1, #0xff00" "\n\t");
#define ftd181 __asm volatile ("mov r1, #0xb500" "\n\t" "movt r1, #0xff00" "\n\t");
#define ftd182 __asm volatile ("mov r1, #0xb600" "\n\t" "movt r1, #0xff00" "\n\t");
#define ftd183 __asm volatile ("mov r1, #0xb700" "\n\t" "movt r1, #0xff00" "\n\t");
#define ftd184 __asm volatile ("mov r1, #0xb800" "\n\t" "movt r1, #0xff00" "\n\t");
#define ftd185 __asm volatile ("mov r1, #0xb900" "\n\t" "movt r1, #0xff00" "\n\t");
#define ftd186 __asm volatile ("mov r1, #0xba00" "\n\t" "movt r1, #0xff00" "\n\t");
#define ftd187 __asm volatile ("mov r1, #0xbb00" "\n\t" "movt r1, #0xff00" "\n\t");
#define ftd188 __asm volatile ("mov r1, #0xbc00" "\n\t" "movt r1, #0xff00" "\n\t");
#define ftd189 __asm volatile ("mov r1, #0xbd00" "\n\t" "movt r1, #0xff00" "\n\t");
#define ftd190 __asm volatile ("mov r1, #0xbe00" "\n\t" "movt r1, #0xff00" "\n\t");
#define ftd191 __asm volatile ("mov r1, #0xbf00" "\n\t" "movt r1, #0xff00" "\n\t");

#define ftd192 __asm volatile ("mov r1, #0xc000" "\n\t" "movt r1, #0xff00" "\n\t");
#define ftd193 __asm volatile ("mov r1, #0xc100" "\n\t" "movt r1, #0xff00" "\n\t");
#define ftd194 __asm volatile ("mov r1, #0xc200" "\n\t" "movt r1, #0xff00" "\n\t");
#define ftd195 __asm volatile ("mov r1, #0xc300" "\n\t" "movt r1, #0xff00" "\n\t");
#define ftd196 __asm volatile ("mov r1, #0xc400" "\n\t" "movt r1, #0xff00" "\n\t");
#define ftd197 __asm volatile ("mov r1, #0xc500" "\n\t" "movt r1, #0xff00" "\n\t");
#define ftd198 __asm volatile ("mov r1, #0xc600" "\n\t" "movt r1, #0xff00" "\n\t");
#define ftd199 __asm volatile ("mov r1, #0xc700" "\n\t" "movt r1, #0xff00" "\n\t");
#define ftd200 __asm volatile ("mov r1, #0xc800" "\n\t" "movt r1, #0xff00" "\n\t");
#define ftd201 __asm volatile ("mov r1, #0xc900" "\n\t" "movt r1, #0xff00" "\n\t");
#define ftd202 __asm volatile ("mov r1, #0xca00" "\n\t" "movt r1, #0xff00" "\n\t");
#define ftd203 __asm volatile ("mov r1, #0xcb00" "\n\t" "movt r1, #0xff00" "\n\t");
#define ftd204 __asm volatile ("mov r1, #0xcc00" "\n\t" "movt r1, #0xff00" "\n\t");
#define ftd205 __asm volatile ("mov r1, #0xcd00" "\n\t" "movt r1, #0xff00" "\n\t");
#define ftd206 __asm volatile ("mov r1, #0xce00" "\n\t" "movt r1, #0xff00" "\n\t");
#define ftd207 __asm volatile ("mov r1, #0xcf00" "\n\t" "movt r1, #0xff00" "\n\t");

#define ftd208 __asm volatile ("mov r1, #0xd000" "\n\t" "movt r1, #0xff00" "\n\t");
#define ftd209 __asm volatile ("mov r1, #0xd100" "\n\t" "movt r1, #0xff00" "\n\t");
#define ftd210 __asm volatile ("mov r1, #0xd200" "\n\t" "movt r1, #0xff00" "\n\t");
#define ftd211 __asm volatile ("mov r1, #0xd300" "\n\t" "movt r1, #0xff00" "\n\t");
#define ftd212 __asm volatile ("mov r1, #0xd400" "\n\t" "movt r1, #0xff00" "\n\t");
#define ftd213 __asm volatile ("mov r1, #0xd500" "\n\t" "movt r1, #0xff00" "\n\t");
#define ftd214 __asm volatile ("mov r1, #0xd600" "\n\t" "movt r1, #0xff00" "\n\t");
#define ftd215 __asm volatile ("mov r1, #0xd700" "\n\t" "movt r1, #0xff00" "\n\t");
#define ftd216 __asm volatile ("mov r1, #0xd800" "\n\t" "movt r1, #0xff00" "\n\t");
#define ftd217 __asm volatile ("mov r1, #0xd900" "\n\t" "movt r1, #0xff00" "\n\t");
#define ftd218 __asm volatile ("mov r1, #0xda00" "\n\t" "movt r1, #0xff00" "\n\t");
#define ftd219 __asm volatile ("mov r1, #0xdb00" "\n\t" "movt r1, #0xff00" "\n\t");
#define ftd220 __asm volatile ("mov r1, #0xdc00" "\n\t" "movt r1, #0xff00" "\n\t");
#define ftd221 __asm volatile ("mov r1, #0xdd00" "\n\t" "movt r1, #0xff00" "\n\t");
#define ftd222 __asm volatile ("mov r1, #0xde00" "\n\t" "movt r1, #0xff00" "\n\t");
#define ftd223 __asm volatile ("mov r1, #0xdf00" "\n\t" "movt r1, #0xff00" "\n\t");

#define ftd224 __asm volatile ("mov r1, #0xe000" "\n\t" "movt r1, #0xff00" "\n\t");
#define ftd225 __asm volatile ("mov r1, #0xe100" "\n\t" "movt r1, #0xff00" "\n\t");
#define ftd226 __asm volatile ("mov r1, #0xe200" "\n\t" "movt r1, #0xff00" "\n\t");
#define ftd227 __asm volatile ("mov r1, #0xe300" "\n\t" "movt r1, #0xff00" "\n\t");
#define ftd228 __asm volatile ("mov r1, #0xe400" "\n\t" "movt r1, #0xff00" "\n\t");
#define ftd229 __asm volatile ("mov r1, #0xe500" "\n\t" "movt r1, #0xff00" "\n\t");
#define ftd230 __asm volatile ("mov r1, #0xe600" "\n\t" "movt r1, #0xff00" "\n\t");
#define ftd231 __asm volatile ("mov r1, #0xe700" "\n\t" "movt r1, #0xff00" "\n\t");
#define ftd232 __asm volatile ("mov r1, #0xe800" "\n\t" "movt r1, #0xff00" "\n\t");
#define ftd233 __asm volatile ("mov r1, #0xe900" "\n\t" "movt r1, #0xff00" "\n\t");
#define ftd234 __asm volatile ("mov r1, #0xea00" "\n\t" "movt r1, #0xff00" "\n\t");
#define ftd235 __asm volatile ("mov r1, #0xeb00" "\n\t" "movt r1, #0xff00" "\n\t");
#define ftd236 __asm volatile ("mov r1, #0xec00" "\n\t" "movt r1, #0xff00" "\n\t");
#define ftd237 __asm volatile ("mov r1, #0xed00" "\n\t" "movt r1, #0xff00" "\n\t");
#define ftd238 __asm volatile ("mov r1, #0xee00" "\n\t" "movt r1, #0xff00" "\n\t");
#define ftd239 __asm volatile ("mov r1, #0xef00" "\n\t" "movt r1, #0xff00" "\n\t");

#define ftd240 __asm volatile ("mov r1, #0xf000" "\n\t" "movt r1, #0xff00" "\n\t");
#define ftd241 __asm volatile ("mov r1, #0xf100" "\n\t" "movt r1, #0xff00" "\n\t");
#define ftd242 __asm volatile ("mov r1, #0xf200" "\n\t" "movt r1, #0xff00" "\n\t");
#define ftd243 __asm volatile ("mov r1, #0xf300" "\n\t" "movt r1, #0xff00" "\n\t");
#define ftd244 __asm volatile ("mov r1, #0xf400" "\n\t" "movt r1, #0xff00" "\n\t");
#define ftd245 __asm volatile ("mov r1, #0xf500" "\n\t" "movt r1, #0xff00" "\n\t");
#define ftd246 __asm volatile ("mov r1, #0xf600" "\n\t" "movt r1, #0xff00" "\n\t");
#define ftd247 __asm volatile ("mov r1, #0xf700" "\n\t" "movt r1, #0xff00" "\n\t");
#define ftd248 __asm volatile ("mov r1, #0xf800" "\n\t" "movt r1, #0xff00" "\n\t");
#define ftd249 __asm volatile ("mov r1, #0xf900" "\n\t" "movt r1, #0xff00" "\n\t");
#define ftd250 __asm volatile ("mov r1, #0xfa00" "\n\t" "movt r1, #0xff00" "\n\t");
#define ftd251 __asm volatile ("mov r1, #0xfb00" "\n\t" "movt r1, #0xff00" "\n\t");
#define ftd252 __asm volatile ("mov r1, #0xfc00" "\n\t" "movt r1, #0xff00" "\n\t");
#define ftd253 __asm volatile ("mov r1, #0xfd00" "\n\t" "movt r1, #0xff00" "\n\t");
#define ftd254 __asm volatile ("mov r1, #0xfe00" "\n\t" "movt r1, #0xff00" "\n\t");
#define ftd255 __asm volatile ("mov r1, #0xff00" "\n\t" "movt r1, #0xff00" "\n\t");

#define setbit __asm volatile ("str r1, [r0]" "\n\t")

void delayFunc(int times)
{    
    while(times>0)
    {times--;}
}
void FastSin()
{
    //__asm volatile ("ldr r0, =0x40021018" "\n\t"); //загрузим в r0 адрес GPIOE->BSRR (32 бита)
    ftd128;setbit;ftd130;setbit;ftd134;setbit;ftd136;setbit;
    ftd140;setbit;ftd144;setbit;ftd146;setbit;ftd150;setbit;
	ftd152;setbit;ftd156;setbit;ftd158;setbit;ftd162;setbit;
    ftd164;setbit;ftd168;setbit;ftd170;setbit;ftd174;setbit;
	ftd176;setbit;ftd178;setbit;ftd180;setbit;ftd182;setbit;
    ftd184;setbit;ftd188;setbit;ftd190;setbit;ftd194;setbit;
	ftd198;setbit;ftd200;setbit;ftd204;setbit;ftd206;setbit;
    ftd208;setbit;ftd210;setbit;ftd214;setbit;ftd216;setbit;
	ftd218;setbit;ftd220;setbit;ftd222;setbit;ftd224;setbit;
    ftd226;setbit;ftd228;setbit;ftd230;setbit;ftd232;setbit;
	ftd234;setbit;ftd236;setbit;ftd236;setbit;ftd238;setbit;
    ftd240;setbit;ftd242;setbit;ftd242;setbit;ftd244;setbit;
	ftd246;setbit;ftd246;setbit;ftd248;setbit;ftd248;setbit;
    ftd250;setbit;ftd250;setbit;ftd252;setbit;ftd252;setbit;
	ftd252;setbit;ftd254;setbit;ftd254;setbit;ftd254;setbit;
    ftd254;setbit;ftd254;setbit;ftd254;setbit;ftd254;setbit;
	ftd255;setbit;ftd254;setbit;ftd254;setbit;ftd254;setbit;
    ftd254;setbit;ftd254;setbit;ftd254;setbit;ftd254;setbit;
	ftd252;setbit;ftd252;setbit;ftd252;setbit;ftd250;setbit;
    ftd250;setbit;ftd248;setbit;ftd248;setbit;ftd246;setbit;
	ftd246;setbit;ftd244;setbit;ftd242;setbit;ftd242;setbit;
    ftd240;setbit;ftd238;setbit;ftd236;setbit;ftd236;setbit;
	ftd234;setbit;ftd232;setbit;ftd230;setbit;ftd228;setbit;
    ftd226;setbit;ftd224;setbit;ftd222;setbit;ftd220;setbit;
	ftd218;setbit;ftd216;setbit;ftd214;setbit;ftd210;setbit;
    ftd208;setbit;ftd206;setbit;ftd204;setbit;ftd200;setbit;
	ftd198;setbit;ftd196;setbit;ftd194;setbit;ftd190;setbit;
    ftd188;setbit;ftd184;setbit;ftd182;setbit;ftd180;setbit;
	ftd178;setbit;ftd174;setbit;ftd170;setbit;ftd168;setbit;
    ftd164;setbit;ftd162;setbit;ftd158;setbit;ftd156;setbit;
	ftd152;setbit;ftd150;setbit;ftd146;setbit;ftd144;setbit;
    ftd140;setbit;ftd138;setbit;ftd134;setbit;ftd130;setbit;
	ftd128;setbit;ftd124;setbit;ftd122;setbit;ftd118;setbit;
    ftd116;setbit;ftd112;setbit;ftd108;setbit;ftd104;setbit;
	ftd102;setbit;ftd100;setbit;ftd98;setbit;ftd94;setbit;
    ftd90;setbit;ftd88;setbit;ftd84;setbit;ftd82;setbit;
	ftd78;setbit;ftd76;setbit;ftd72;setbit;ftd70;setbit;
    ftd68;setbit;ftd64;setbit;ftd62;setbit;ftd60;setbit;
	ftd56;setbit;ftd54;setbit;ftd52;setbit;ftd50;setbit;
    ftd46;setbit;ftd44;setbit;ftd42;setbit;ftd40;setbit;
	ftd38;setbit;ftd36;setbit;ftd34;setbit;ftd30;setbit;
    ftd28;setbit;ftd26;setbit;ftd25;setbit;ftd24;setbit;
	ftd22;setbit;ftd20;setbit;ftd18;setbit;ftd16;setbit;
    ftd15;setbit;ftd14;setbit;ftd12;setbit;ftd10;setbit;
	ftd9;setbit;ftd8;setbit;ftd8;setbit;ftd7;setbit;
    ftd6;setbit;ftd5;setbit;ftd4;setbit;ftd4;setbit;
	ftd3;setbit;ftd2;setbit;ftd2;setbit;ftd1;setbit;
    ftd0;setbit;ftd0;setbit;ftd0;setbit;ftd0;setbit;
    ftd0;setbit;ftd0;setbit;ftd0;setbit;ftd0;setbit;
    ftd0;setbit;ftd0;setbit;ftd1;setbit;ftd2;setbit;
	ftd2;setbit;ftd3;setbit;ftd4;setbit;ftd4;setbit;
    ftd5;setbit;ftd6;setbit;ftd7;setbit;ftd8;setbit;
	ftd9;setbit;ftd10;setbit;ftd12;setbit;ftd14;setbit;
    ftd15;setbit;ftd16;setbit;ftd18;setbit;ftd20;setbit;
	ftd22;setbit;ftd24;setbit;ftd26;setbit;ftd27;setbit;
    ftd28;setbit;ftd30;setbit;ftd34;setbit;ftd38;setbit;
	ftd39;setbit;ftd40;setbit;ftd42;setbit;ftd44;setbit;
    ftd46;setbit;ftd50;setbit;ftd52;setbit;ftd54;setbit;
	ftd56;setbit;ftd60;setbit;ftd62;setbit;ftd64;setbit;
    ftd68;setbit;ftd70;setbit;ftd72;setbit;ftd76;setbit;
	ftd78;setbit;ftd82;setbit;ftd84;setbit;ftd88;setbit;
    ftd90;setbit;ftd94;setbit;ftd96;setbit;ftd100;setbit;
	ftd102;setbit;ftd106;setbit;ftd108;setbit;ftd112;setbit;
    ftd116;setbit;ftd118;setbit;ftd122;setbit;ftd124;setbit;
}

#endif //R2R_H_