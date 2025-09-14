
build/Template.elf:	file format elf32-littlearm

Disassembly of section .text:

03800000 <vexCodeSig>:
 3800000: 35585658     	ldrblo	r5, [r8, #-0x658]
 3800004: 00000000     	andeq	r0, r0, r0
 3800008: 00000001     	andeq	r0, r0, r1
		...

03800020 <vexStartup>:
 3800020: e92d4830     	push	{r4, r5, r11, lr}
 3800024: e3040010     	movw	r0, #0x4010
 3800028: e3041010     	movw	r1, #0x4010
 380002c: e3400380     	movt	r0, #0x380
 3800030: e3401380     	movt	r1, #0x380
 3800034: e5900000     	ldr	r0, [r0]
 3800038: e5914000     	ldr	r4, [r1]
 380003c: e1540000     	cmp	r4, r0
 3800040: 2a00000a     	bhs	0x3800070 <vexStartup+0x50> @ imm = #0x28
 3800044: e2841004     	add	r1, r4, #4
 3800048: e1500001     	cmp	r0, r1
 380004c: 81a01000     	movhi	r1, r0
 3800050: e1e00004     	mvn	r0, r4
 3800054: e0810000     	add	r0, r1, r0
 3800058: e3c00003     	bic	r0, r0, #3
 380005c: e2805004     	add	r5, r0, #4
 3800060: e1a00004     	mov	r0, r4
 3800064: e1a01005     	mov	r1, r5
 3800068: eb000051     	bl	0x38001b4 <__aeabi_memclr8> @ imm = #0x144
 380006c: e0844005     	add	r4, r4, r5
 3800070: e3000000     	movw	r0, #0x0
 3800074: e3001000     	movw	r1, #0x0
 3800078: e3400000     	movt	r0, #0x0
 380007c: e3401000     	movt	r1, #0x0
 3800080: e5900000     	ldr	r0, [r0]
 3800084: e5911000     	ldr	r1, [r1]
 3800088: e1510000     	cmp	r1, r0
 380008c: 28bd8830     	pophs	{r4, r5, r11, pc}
 3800090: e3a00000     	mov	r0, #0
 3800094: e4840004     	str	r0, [r4], #4
 3800098: eafffffd     	b	0x3800094 <vexStartup+0x74> @ imm = #-0xc
 380009c: d4d4d4d4     	ldrble	sp, [r4], #1236
 38000a0: d4d4d4d4     	ldrble	sp, [r4], #1236
 38000a4: d4d4d4d4     	ldrble	sp, [r4], #1236
 38000a8: d4d4d4d4     	ldrble	sp, [r4], #1236
 38000ac: d4d4d4d4     	ldrble	sp, [r4], #1236
 38000b0: d4d4d4d4     	ldrble	sp, [r4], #1236
 38000b4: d4d4d4d4     	ldrble	sp, [r4], #1236
 38000b8: d4d4d4d4     	ldrble	sp, [r4], #1236
 38000bc: d4d4d4d4     	ldrble	sp, [r4], #1236

038000c0 <__aeabi_memset8>:
 38000c0: e3100003     	tst	r0, #3
 38000c4: e92d4010     	push	{r4, lr}
 38000c8: 0a000037     	beq	0x38001ac <__aeabi_memset8+0xec> @ imm = #0xdc
 38000cc: e3510000     	cmp	r1, #0
 38000d0: e2411001     	sub	r1, r1, #1
 38000d4: 0a000032     	beq	0x38001a4 <__aeabi_memset8+0xe4> @ imm = #0xc8
 38000d8: e202c0ff     	and	r12, r2, #255
 38000dc: e1a03000     	mov	r3, r0
 38000e0: ea000002     	b	0x38000f0 <__aeabi_memset8+0x30> @ imm = #0x8
 38000e4: e3510000     	cmp	r1, #0
 38000e8: e2411001     	sub	r1, r1, #1
 38000ec: 0a00002c     	beq	0x38001a4 <__aeabi_memset8+0xe4> @ imm = #0xb0
 38000f0: e4c3c001     	strb	r12, [r3], #1
 38000f4: e3130003     	tst	r3, #3
 38000f8: 1afffff9     	bne	0x38000e4 <__aeabi_memset8+0x24> @ imm = #-0x1c
 38000fc: e3510003     	cmp	r1, #3
 3800100: 9a000020     	bls	0x3800188 <__aeabi_memset8+0xc8> @ imm = #0x80
 3800104: e202e0ff     	and	lr, r2, #255
 3800108: e18ee40e     	orr	lr, lr, lr, lsl #8
 380010c: e351000f     	cmp	r1, #15
 3800110: e18ee80e     	orr	lr, lr, lr, lsl #16
 3800114: 9a000010     	bls	0x380015c <__aeabi_memset8+0x9c> @ imm = #0x40
 3800118: e1a04001     	mov	r4, r1
 380011c: e283c010     	add	r12, r3, #16
 3800120: e2444010     	sub	r4, r4, #16
 3800124: e354000f     	cmp	r4, #15
 3800128: e50ce010     	str	lr, [r12, #-0x10]
 380012c: e50ce00c     	str	lr, [r12, #-0xc]
 3800130: e50ce008     	str	lr, [r12, #-0x8]
 3800134: e50ce004     	str	lr, [r12, #-0x4]
 3800138: e28cc010     	add	r12, r12, #16
 380013c: 8afffff7     	bhi	0x3800120 <__aeabi_memset8+0x60> @ imm = #-0x24
 3800140: e241c010     	sub	r12, r1, #16
 3800144: e3ccc00f     	bic	r12, r12, #15
 3800148: e201100f     	and	r1, r1, #15
 380014c: e28cc010     	add	r12, r12, #16
 3800150: e3510003     	cmp	r1, #3
 3800154: e083300c     	add	r3, r3, r12
 3800158: 9a00000a     	bls	0x3800188 <__aeabi_memset8+0xc8> @ imm = #0x28
 380015c: e1a04003     	mov	r4, r3
 3800160: e1a0c001     	mov	r12, r1
 3800164: e24cc004     	sub	r12, r12, #4
 3800168: e35c0003     	cmp	r12, #3
 380016c: e484e004     	str	lr, [r4], #4
 3800170: 8afffffb     	bhi	0x3800164 <__aeabi_memset8+0xa4> @ imm = #-0x14
 3800174: e241c004     	sub	r12, r1, #4
 3800178: e3ccc003     	bic	r12, r12, #3
 380017c: e28cc004     	add	r12, r12, #4
 3800180: e083300c     	add	r3, r3, r12
 3800184: e2011003     	and	r1, r1, #3
 3800188: e3510000     	cmp	r1, #0
 380018c: 120220ff     	andne	r2, r2, #255
 3800190: 10831001     	addne	r1, r3, r1
 3800194: 0a000002     	beq	0x38001a4 <__aeabi_memset8+0xe4> @ imm = #0x8
 3800198: e4c32001     	strb	r2, [r3], #1
 380019c: e1530001     	cmp	r3, r1
 38001a0: 1afffffc     	bne	0x3800198 <__aeabi_memset8+0xd8> @ imm = #-0x10
 38001a4: e8bd4010     	pop	{r4, lr}
 38001a8: e12fff1e     	bx	lr
 38001ac: e1a03000     	mov	r3, r0
 38001b0: eaffffd1     	b	0x38000fc <__aeabi_memset8+0x3c> @ imm = #-0xbc

038001b4 <__aeabi_memclr8>:
 38001b4: e3a02000     	mov	r2, #0
 38001b8: eaffffc0     	b	0x38000c0 <__aeabi_memset8> @ imm = #-0x100

Disassembly of section .ARM.exidx:

03804000 <__exidx_start>:
 3804000: 7fffc020     	svcvc	#0xffc020
 3804004: 00000001     	andeq	r0, r0, r1
 3804008: 7fffc1b4     	svcvc	#0xffc1b4
 380400c: 00000001     	andeq	r0, r0, r1

Disassembly of section .ARM.attributes:

00000000 <__ARM.attributes_start>:
       0: 00003941     	andeq	r3, r0, r1, asr #18
       4: 61656100     	<unknown>
       8: 01006962     	<unknown>
       c: 0000002f     	andeq	r0, r0, pc, lsr #32
      10: 302e3243     	eorlo	r3, lr, r3, asr #4
      14: 0a060039     	beq	0x180100 <__ARM.attributes_end+0x1800c6> @ imm = #0x1800e4
      18: 01084107     	<unknown>
      1c: 030a0209     	movweq	r0, #0xa209
      20: 000e010c     	andeq	r0, lr, r12, lsl #2
      24: 04120111     	ldreq	r0, [r2], #-273
      28: 01150114     	tsteq	r5, r4, lsl r1
      2c: 01180317     	tsteq	r8, r7, lsl r3
      30: 011a0119     	tsteq	r10, r9, lsl r1
      34: 0122031e     	<unknown>
      38: 26 01        	<unknown>

Disassembly of section .heap:

03804010 <_heap_start>:
...

Disassembly of section .stack:

03904010 <_stack_end>:
...

03906010 <_stack>:
...

03906410 <_supervisor_stack_end>:
...

03906c10 <_abort_stack_end>:
...

03907010 <_fiq_stack_end>:
...

03907410 <_undef_stack_end>:
...

Disassembly of section .debug_abbrev:

00000000 <.debug_abbrev>:
       0: 25011101     	strhs	r1, [r1, #-0x101]
       4: 03051325     	movweq	r1, #0x5325
       8: 10177225     	andsne	r7, r7, r5, lsr #4
       c: 11251b17     	<unknown>
      10: 7306121b     	movwvc	r1, #0x621b
      14: 02000017     	andeq	r0, r0, #23
      18: 1b11012e     	blne	0x4404d8 <.debug_abbrev+0x4404d8> @ imm = #0x4404b8
      1c: 18400612     	stmdane	r0, {r1, r4, r9, r10} ^
      20: 2503197a     	strhs	r1, [r3, #-0x97a]
      24: 0b3b0b3a     	bleq	0xec2d14 <.debug_abbrev+0xec2d14> @ imm = #0xec2ce8
      28: 193f1927     	ldmdbne	pc!, {r0, r1, r2, r5, r8, r11, r12}
      2c: 05030000     	streq	r0, [r3, #-0x0]
      30: 03180200     	tsteq	r8, #0, #4
      34: 3b0b3a25     	bllo	0x2ce8d0 <.debug_abbrev+0x2ce8d0> @ imm = #0x2ce894
      38: 0013490b     	andseq	r4, r3, r11, lsl #18
      3c: 01480400     	mrseq	r0, spsr
      40: 0182137f     	orreq	r1, r2, pc, ror r3
      44: 1b018119     	blne	0x604b0 <.debug_abbrev+0x604b0> @ imm = #0x60464
      48: 49050000     	<unknown>
      4c: 7e180200     	cdpvc	p2, #0x1, c0, c8, c0, #0x0
      50: 06000018     	<unknown>
      54: 2503012e     	strhs	r0, [r3, #-0x12e]
      58: 0b3b0b3a     	bleq	0xec2d48 <.debug_abbrev+0xec2d48> @ imm = #0xec2ce8
      5c: 193c1927     	ldmdbne	r12!, {r0, r1, r2, r5, r8, r11, r12}
      60: 0000193f     	andeq	r1, r0, pc, lsr r9
      64: 49000507     	stmdbmi	r0, {r0, r1, r2, r8, r10}
      68: 08000013     	stmdaeq	r0, {r0, r1, r4}
      6c: 0000000f     	andeq	r0, r0, pc
      70: 49001609     	stmdbmi	r0, {r0, r3, r9, r10, r12}
      74: 3a250313     	blo	0x940cc8 <.debug_abbrev+0x940cc8> @ imm = #0x940c4c
      78: 000b3b0b     	andeq	r3, r11, r11, lsl #22
      7c: 00240a00     	eoreq	r0, r4, r0, lsl #20
      80: 0b3e2503     	bleq	0xf89494 <.debug_abbrev+0xf89494> @ imm = #0xf8940c
      84: 00000b0b     	andeq	r0, r0, r11, lsl #22
      88: 01110100     	tsteq	r1, r0, lsl #2
      8c: 01111710     	tsteq	r1, r0, lsl r7
      90: 08030112     	stmdaeq	r3, {r1, r4, r8}
      94: 0825081b     	stmdaeq	r5!, {r0, r1, r3, r4, r11}
      98: 00000513     	andeq	r0, r0, r3, lsl r5
      9c: 03000a02     	movweq	r0, #0xa02
      a0: 3b063a08     	bllo	0x18e8c8 <.debug_abbrev+0x18e8c8> @ imm = #0x18e820
      a4: 00011106     	andeq	r1, r1, r6, lsl #2
      a8: 00 00        	<unknown>

Disassembly of section .debug_info:

00000000 <.debug_info>:
       0: 0000007a     	andeq	r0, r0, r10, ror r0
       4: 04010005     	streq	r0, [r1], #-5
       8: 00000000     	andeq	r0, r0, r0
       c: 001d0001     	andseq	r0, sp, r1
      10: 00000801     	andeq	r0, r0, r1, lsl #16
      14: 00000000     	andeq	r0, r0, r0
      18: 08000200     	stmdaeq	r0, {r9}
      1c: 08000000     	<unknown>
      20: 02000000     	andeq	r0, r0, #0
      24: 00000800     	andeq	r0, r0, r0, lsl #16
      28: 075d0100     	ldrbeq	r0, [sp, -r0, lsl #2]
      2c: 01032901     	mrseq	r2, apsr
      30: 29010850     	stmdbhs	r1, {r4, r6, r11}
      34: 0000006c     	andeq	r0, r0, r12, rrx
      38: 09510103     	ldmdbeq	r1, {r0, r1, r8} ^
      3c: 006d2901     	rsbeq	r2, sp, r1, lsl #18
      40: 58040000     	<unknown>
      44: 01000000     	<unknown>
      48: 03500105     	cmpeq	r0, #1073741825
      4c: 055001a3     	ldrbeq	r0, [r0, #-0x1a3]
      50: a3035101     	movwge	r5, #0x3101
      54: 00005101     	andeq	r5, r0, r1, lsl #2
      58: 2b010306     	blhs	0x40c78 <.debug_info+0x40c78> @ imm = #0x40c18
      5c: 00006c07     	andeq	r6, r0, r7, lsl #24
      60: 006d0700     	rsbeq	r0, sp, r0, lsl #14
      64: 79070000     	<unknown>
      68: 00000000     	andeq	r0, r0, r0
      6c: 00750908     	rsbseq	r0, r5, r8, lsl #18
      70: 02050000     	andeq	r0, r5, #0
      74: 07040a12     	smladeq	r4, r2, r10, r0
      78: 05060a04     	streq	r0, [r6, #-0xa04]
      7c: 01760004     	cmneq	r6, r4
      80: 00050000     	andeq	r0, r5, r0
      84: 00890401     	addeq	r0, r9, r1, lsl #8
      88: 83010000     	movwhi	r0, #0x1000
      8c: c0000000     	andgt	r0, r0, r0
      90: b4038000     	strlt	r8, [r3], #-0
      94: 2f038001     	svchs	#0x38001
      98: 656d6f68     	strbvs	r6, [sp, #-0xf68]!
      9c: 6e656a2f     	vmulvs.f32	s13, s10, s31
      a0: 736e696b     	<unknown>
      a4: 6567612f     	strbvs	r6, [r7, #-0x12f]!
      a8: 772f746e     	strvc	r7, [pc, -lr, ror #8]!
      ac: 736b726f     	<unknown>
      b0: 65636170     	strbvs	r6, [r3, #-0x170]!
      b4: 6674612f     	ldrbtvs	r6, [r4], -pc, lsr #2
      b8: 30322f65     	eorslo	r2, r2, r5, ror #30
      bc: 622f782e     	eorvs	r7, pc, #3014656
      c0: 646c6975     	strbtvs	r6, [r12], #-2421
      c4: 6674612d     	ldrbtvs	r6, [r4], -sp, lsr #2
      c8: 61702d65     	cmnvs	r0, r5, ror #26
      cc: 67616b63     	strbvs	r6, [r1, -r3, ror #22]!
      d0: 75622f65     	strbvc	r2, [r2, #-0xf65]!
      d4: 5f646c69     	svcpl	#0x646c69
      d8: 6c77656e     	ldclvs	p5, c6, [r7], #-440
      dc: 6f5f6269     	svcvs	#0x5f6269
      e0: 6c726576     	ldclvs	p5, c6, [r2], #-472
      e4: 5f2f7961     	svcpl	#0x2f7961
      e8: 73706564     	cmnvc	r0, #100, #10
      ec: 77656e2f     	strbvc	r6, [r5, -pc, lsr #28]!
      f0: 2d62696c     	stclhs	p9, c6, [r2, #-432]!
      f4: 2f637273     	svchs	#0x637273
      f8: 6c77656e     	ldclvs	p5, c6, [r7], #-440
      fc: 6c2f6269     	stcvs	p2, c6, [pc], #-420
     100: 2f636269     	svchs	#0x636269
     104: 6863616d     	stmdavs	r3!, {r0, r2, r3, r5, r6, r8, sp, lr} ^
     108: 2f656e69     	svchs	#0x656e69
     10c: 2f6d7261     	svchs	#0x6d7261
     110: 62616561     	rsbvs	r6, r1, #406847488
     114: 656d5f69     	strbvs	r5, [sp, #-0xf69]!
     118: 7465736d     	strbtvc	r7, [r5], #-877
     11c: 6d72612d     	ldclvs	p1, c6, [r2, #-180]!
     120: 2f00532e     	svchs	#0x532e
     124: 656d6f68     	strbvs	r6, [sp, #-0xf68]!
     128: 6e656a2f     	vmulvs.f32	s13, s10, s31
     12c: 736e696b     	<unknown>
     130: 6567612f     	strbvs	r6, [r7, #-0x12f]!
     134: 772f746e     	strvc	r7, [pc, -lr, ror #8]!
     138: 736b726f     	<unknown>
     13c: 65636170     	strbvs	r6, [r3, #-0x170]!
     140: 6674612f     	ldrbtvs	r6, [r4], -pc, lsr #2
     144: 30322f65     	eorslo	r2, r2, r5, ror #30
     148: 622f782e     	eorvs	r7, pc, #3014656
     14c: 646c6975     	strbtvs	r6, [r12], #-2421
     150: 6674612d     	ldrbtvs	r6, [r4], -sp, lsr #2
     154: 61702d65     	cmnvs	r0, r5, ror #26
     158: 67616b63     	strbvs	r6, [r1, -r3, ror #22]!
     15c: 75622f65     	strbvc	r2, [r2, #-0xf65]!
     160: 5f646c69     	svcpl	#0x646c69
     164: 6c77656e     	ldclvs	p5, c6, [r7], #-440
     168: 6f5f6269     	svcvs	#0x5f6269
     16c: 6c726576     	ldclvs	p5, c6, [r2], #-472
     170: 6d2f7961     	stcvs	p9, c7, [pc, #-388]!
     174: 69746c75     	ldmdbvs	r4!, {r0, r2, r4, r5, r6, r10, r11, sp, lr} ^
     178: 2d62696c     	stclhs	p9, c6, [r2, #-432]!
     17c: 6c697562     	stclvs	p5, c7, [r9], #-392
     180: 6e2f7364     	cdpvs	p3, #0x2, c7, c15, c4, #0x3
     184: 696c7765     	stmdbvs	r12!, {r0, r2, r5, r6, r8, r9, r10, r12, sp, lr} ^
     188: 72612f62     	rsbvc	r2, r1, #392
     18c: 6137766d     	teqvs	r7, sp, ror #12
     190: 666f735f     	uqsaxvs	r7, pc, pc
     194: 66765f74     	uhsub16vs	r5, r6, r4
     198: 5f337670     	svcpl	#0x337670
     19c: 5f363164     	svcpl	#0x363164
     1a0: 6c616e75     	stclvs	p14, c6, [r1], #-468
     1a4: 656e6769     	strbvs	r6, [lr, #-0x769]!
     1a8: 75622f64     	strbvc	r2, [r2, #-0xf64]!
     1ac: 2f646c69     	svchs	#0x646c69
     1b0: 2d6d7261     	stclhs	p2, c7, [sp, #-388]!
     1b4: 656e6f6e     	strbvs	r6, [lr, #-0xf6e]!
     1b8: 6261652d     	rsbvs	r6, r1, #188743680
     1bc: 656e2f69     	strbvs	r2, [lr, #-0xf69]!
     1c0: 62696c77     	rsbvs	r6, r9, #30464
     1c4: 616c6300     	<unknown>
     1c8: 7620676e     	strtvc	r6, [r0], -lr, ror #14
     1cc: 69737265     	ldmdbvs	r3!, {r0, r2, r5, r6, r9, r12, sp, lr} ^
     1d0: 32206e6f     	eorlo	r6, r0, #1776
     1d4: 2e312e30     	mrchs	p14, #0x1, r2, c1, c0, #0x1
     1d8: 80010030     	andhi	r0, r1, r0, lsr r0
     1dc: 65615f02     	strbvs	r5, [r1, #-0xf02]!
     1e0: 5f696261     	svcpl	#0x696261
     1e4: 736d656d     	<unknown>
     1e8: 00007465     	andeq	r7, r0, r5, ror #8
     1ec: 1b000000     	blne	0x1f4 <.debug_info+0x1f4> @ imm = #0x0
     1f0: c0000000     	andgt	r0, r0, r0
     1f4: 00038000     	andeq	r8, r3, r0

Disassembly of section .debug_str_offsets:

00000000 <.debug_str_offsets>:
       0: 0000002c     	andeq	r0, r0, r12, lsr #32
       4: 00000005     	andeq	r0, r0, r5
       8: 0000008a     	andeq	r0, r0, r10, lsl #1
       c: 00000002     	andeq	r0, r0, r2
      10: 0000008b     	andeq	r0, r0, r11, lsl #1
      14: 0000014c     	andeq	r0, r0, r12, asr #2
      18: 0000015b     	andeq	r0, r0, r11, asr r1
      1c: 0000012d     	andeq	r0, r0, sp, lsr #2
      20: 00000148     	andeq	r0, r0, r8, asr #2
      24: 00000139     	andeq	r0, r0, r9, lsr r1
      28: 00000134     	andeq	r0, r0, r4, lsr r1
      2c: 00000000     	andeq	r0, r0, r0

Disassembly of section .debug_str:

00000000 <.debug_str>:
       0: 682f006e     	stmdavs	pc!, {r1, r2, r3, r5, r6}
       4: 2f656d6f     	svchs	#0x656d6f
       8: 6b6e656a     	blvs	0x1b995b8 <.debug_info+0x1b995b8> @ imm = #0x1b995a8
       c: 2f736e69     	svchs	#0x736e69
      10: 6e656761     	cdpvs	p7, #0x6, c6, c5, c1, #0x3
      14: 6f772f74     	svcvs	#0x772f74
      18: 70736b72     	rsbsvc	r6, r3, r2, ror r11
      1c: 2f656361     	svchs	#0x656361
      20: 65667461     	strbvs	r7, [r6, #-0x461]!
      24: 2e30322f     	cdphs	p2, #0x3, c3, c0, c15, #0x1
      28: 75622f78     	strbvc	r2, [r2, #-0xf78]!
      2c: 2d646c69     	stclhs	p12, c6, [r4, #-420]!
      30: 65667461     	strbvs	r7, [r6, #-0x461]!
      34: 6361702d     	<unknown>
      38: 6567616b     	strbvs	r6, [r7, #-0x16b]!
      3c: 6975622f     	ldmdbvs	r5!, {r0, r1, r2, r3, r5, r9, sp, lr} ^
      40: 6e5f646c     	cdpvs	p4, #0x5, c6, c15, c12, #0x3
      44: 696c7765     	stmdbvs	r12!, {r0, r2, r5, r6, r8, r9, r10, r12, sp, lr} ^
      48: 766f5f62     	strbtvc	r5, [pc], -r2, ror #30
      4c: 616c7265     	<unknown>
      50: 645f2f79     	ldrbvs	r2, [pc], #-3961
      54: 2f737065     	svchs	#0x737065
      58: 6c77656e     	ldclvs	p5, c6, [r7], #-440
      5c: 732d6269     	<unknown>
      60: 6e2f6372     	mcrvs	p3, #0x1, r6, c15, c2, #0x3
      64: 696c7765     	stmdbvs	r12!, {r0, r2, r5, r6, r8, r9, r10, r12, sp, lr} ^
      68: 696c2f62     	stmdbvs	r12!, {r1, r5, r6, r8, r9, r10, r11, sp} ^
      6c: 6d2f6362     	stcvs	p3, c6, [pc, #-392]!
      70: 69686361     	stmdbvs	r8!, {r0, r5, r6, r8, r9, sp, lr} ^
      74: 612f656e     	<unknown>
      78: 612f6d72     	<unknown>
      7c: 69626165     	stmdbvs	r2!, {r0, r2, r5, r6, r8, sp, lr} ^
      80: 6d656d5f     	stclvs	p13, c6, [r5, #-380]!
      84: 2e726c63     	cdphs	p12, #0x7, c6, c2, c3, #0x3
      88: 2f000063     	svchs	#0x63
      8c: 656d6f68     	strbvs	r6, [sp, #-0xf68]!
      90: 6e656a2f     	vmulvs.f32	s13, s10, s31
      94: 736e696b     	<unknown>
      98: 6567612f     	strbvs	r6, [r7, #-0x12f]!
      9c: 772f746e     	strvc	r7, [pc, -lr, ror #8]!
      a0: 736b726f     	<unknown>
      a4: 65636170     	strbvs	r6, [r3, #-0x170]!
      a8: 6674612f     	ldrbtvs	r6, [r4], -pc, lsr #2
      ac: 30322f65     	eorslo	r2, r2, r5, ror #30
      b0: 622f782e     	eorvs	r7, pc, #3014656
      b4: 646c6975     	strbtvs	r6, [r12], #-2421
      b8: 6674612d     	ldrbtvs	r6, [r4], -sp, lsr #2
      bc: 61702d65     	cmnvs	r0, r5, ror #26
      c0: 67616b63     	strbvs	r6, [r1, -r3, ror #22]!
      c4: 75622f65     	strbvc	r2, [r2, #-0xf65]!
      c8: 5f646c69     	svcpl	#0x646c69
      cc: 6c77656e     	ldclvs	p5, c6, [r7], #-440
      d0: 6f5f6269     	svcvs	#0x5f6269
      d4: 6c726576     	ldclvs	p5, c6, [r2], #-472
      d8: 6d2f7961     	stcvs	p9, c7, [pc, #-388]!
      dc: 69746c75     	ldmdbvs	r4!, {r0, r2, r4, r5, r6, r10, r11, sp, lr} ^
      e0: 2d62696c     	stclhs	p9, c6, [r2, #-432]!
      e4: 6c697562     	stclvs	p5, c7, [r9], #-392
      e8: 6e2f7364     	cdpvs	p3, #0x2, c7, c15, c4, #0x3
      ec: 696c7765     	stmdbvs	r12!, {r0, r2, r5, r6, r8, r9, r10, r12, sp, lr} ^
      f0: 72612f62     	rsbvc	r2, r1, #392
      f4: 6137766d     	teqvs	r7, sp, ror #12
      f8: 666f735f     	uqsaxvs	r7, pc, pc
      fc: 66765f74     	uhsub16vs	r5, r6, r4
     100: 5f337670     	svcpl	#0x337670
     104: 5f363164     	svcpl	#0x363164
     108: 6c616e75     	stclvs	p14, c6, [r1], #-468
     10c: 656e6769     	strbvs	r6, [lr, #-0x769]!
     110: 75622f64     	strbvc	r2, [r2, #-0xf64]!
     114: 2f646c69     	svchs	#0x646c69
     118: 2d6d7261     	stclhs	p2, c7, [sp, #-388]!
     11c: 656e6f6e     	strbvs	r6, [lr, #-0xf6e]!
     120: 6261652d     	rsbvs	r6, r1, #188743680
     124: 656e2f69     	strbvs	r2, [lr, #-0xf69]!
     128: 62696c77     	rsbvs	r6, r9, #30464
     12c: 7a697300     	bvc	0x1a5cd34 <.debug_info+0x1a5cd34> @ imm = #0x1a5cc00
     130: 00745f65     	rsbseq	r5, r4, r5, ror #30
     134: 74736564     	ldrbtvc	r6, [r3], #-1380
     138: 615f5f00     	cmpvs	pc, r0, lsl #30
     13c: 69626165     	stmdbvs	r2!, {r0, r2, r5, r6, r8, sp, lr} ^
     140: 6d656d5f     	stclvs	p13, c6, [r5, #-380]!
     144: 00726c63     	rsbseq	r6, r2, r3, ror #24
     148: 00746e69     	rsbseq	r6, r4, r9, ror #28
     14c: 65615f5f     	strbvs	r5, [r1, #-0xf5f]!
     150: 5f696261     	svcpl	#0x696261
     154: 736d656d     	<unknown>
     158: 75007465     	strvc	r7, [r0, #-0x465]
     15c: 6769736e     	strbvs	r7, [r9, -lr, ror #6]!
     160: 2064656e     	rsbhs	r6, r4, lr, ror #10
     164: 00746e69     	rsbseq	r6, r4, r9, ror #28

Disassembly of section .debug_addr:

00000000 <.debug_addr>:
       0: 0000000c     	andeq	r0, r0, r12
       4: 00040005     	andeq	r0, r4, r5
       8: 038001b4     	orreq	r0, r0, #180, #2
       c: 038001b8     	orreq	r0, r0, #184, #2

Disassembly of section .debug_frame:

00000000 <.debug_frame>:
       0: 00000010     	andeq	r0, r0, r0, lsl r0
       4: ffffffff     	<unknown>
       8: 00040004     	andeq	r0, r4, r4
       c: 0c0e7c01     	<unknown>
      10: 0000000d     	andeq	r0, r0, sp
      14: 0000000c     	andeq	r0, r0, r12
      18: 00000000     	andeq	r0, r0, r0
      1c: 038001b4     	orreq	r0, r0, #180, #2
      20: 00000008     	andeq	r0, r0, r8

Disassembly of section .debug_line:

00000000 <.debug_line>:
       0: 0000007f     	andeq	r0, r0, pc, ror r0
       4: 00040005     	andeq	r0, r4, r5
       8: 00000065     	andeq	r0, r0, r5, rrx
       c: fb010101     	blx	0x4041a <.debug_info+0x4041a> @ imm = #0x40406
      10: 01000d0e     	<unknown>
      14: 00010101     	andeq	r0, r1, r1, lsl #2
      18: 00010000     	andeq	r0, r1, r0
      1c: 01010100     	mrseq	r0, apsr
      20: 014d021f     	<unknown>
      24: 01ef0000     	mvneq	r0, r0
      28: 01030000     	mrseq	r0, apsr
      2c: 050f021f     	streq	r0, [pc, #-0x21f]
      30: 00c5031e     	sbceq	r0, r5, lr, lsl r3
      34: 74000000     	strvc	r0, [r0], #-0
      38: c8892b78     	stmgt	r9, {r3, r4, r5, r6, r8, r9, r11, sp}
      3c: b838302d     	ldmdalt	r8!, {r0, r2, r3, r5, r12, sp}
      40: ac03d40a     	<unknown>
      44: 8dc2c8c7     	stclhi	p8, c12, [r2, #796]
      48: 01000000     	<unknown>
      4c: 892b7874     	stmdbhi	r11!, {r2, r4, r5, r6, r11, r12, sp, lr}
      50: 38302dc8     	ldmdalo	r0!, {r3, r6, r7, r8, r10, r11, sp}
      54: 03d40ab8     	bicseq	r0, r4, #184, #20
      58: c2c8c7ac     	sbcgt	r12, r8, #172, #14
      5c: 00000252     	andeq	r0, r0, r2, asr r2
      60: e8442c01     	stmda	r4, {r0, r10, r11, sp} ^
      64: 95b1a221     	ldrls	r10, [r1, #0x221]!
      68: b02ede1c     	eorlt	sp, lr, r12, lsl lr
      6c: 68652efb     	stmdavs	r5!, {r0, r1, r3, r4, r5, r6, r7, r9, r10, r11, sp} ^
      70: 0a030567     	beq	0xc1614 <.debug_info+0xc1614> @ imm = #0xc159c
      74: b4020500     	strlt	r0, [r2], #-1280
      78: 03038001     	movweq	r8, #0x3001
      7c: 0802012b     	stmdaeq	r2, {r0, r1, r3, r5, r8}
      80: 81010100     	mrshi	r0, apsr
      84: 05000000     	streq	r0, [r0, #-0x0]
      88: 2e000400     	cdphs	p4, #0x0, c0, c0, c0, #0x0
      8c: 01000000     	<unknown>
      90: 0efb0101     	cdpeq	p1, #0xf, c0, c11, c1, #0x0
      94: 0101000d     	mrseq	r0, apsr
      98: 00000101     	andeq	r0, r0, r1, lsl #2
      9c: 00000100     	andeq	r0, r0, r0, lsl #2
      a0: 1f010101     	svcne	#0x10101
      a4: 00014d02     	andeq	r4, r1, r2, lsl #26
      a8: 00027e00     	andeq	r7, r2, r0, lsl #28
      ac: 1f010200     	svcne	#0x10200
      b0: 00020f02     	andeq	r0, r2, r2, lsl #30
      b4: 00000000     	andeq	r0, r0, r0
      b8: 0000023f     	andeq	r0, r0, pc, lsr r2
      bc: 02050001     	andeq	r0, r5, #1
      c0: 038000c0     	orreq	r0, r0, #192
      c4: 4b012303     	blmi	0x48cd8 <.debug_info+0x48cd8> @ imm = #0x48c0c
      c8: 4b4b4b4b     	blmi	0x12d2dfc <.debug_info+0x12d2dfc> @ imm = #0x12d2d2c
      cc: 4c4b4b4b     	<unknown>
      d0: 4b4c4b4b     	blmi	0x1312e04 <.debug_info+0x1312e04> @ imm = #0x1312d2c
      d4: 4b4b4c4b     	blmi	0x12d3208 <.debug_info+0x12d3208> @ imm = #0x12d312c
      d8: 4b4b4b4b     	blmi	0x12d2e0c <.debug_info+0x12d2e0c> @ imm = #0x12d2d2c
      dc: 4b4c4b4b     	blmi	0x1312e10 <.debug_info+0x1312e10> @ imm = #0x1312d2c
      e0: 4b4b4b4b     	blmi	0x12d2e14 <.debug_info+0x12d2e14> @ imm = #0x12d2d2c
      e4: 4b4b4b4b     	blmi	0x12d2e18 <.debug_info+0x12d2e18> @ imm = #0x12d2d2c
      e8: 4b4b4b4b     	blmi	0x12d2e1c <.debug_info+0x12d2e1c> @ imm = #0x12d2d2c
      ec: 4c4b4c4b     	mcrrmi	p12, #0x4, r4, r11, c11
      f0: 4b4b4b4b     	blmi	0x12d2e24 <.debug_info+0x12d2e24> @ imm = #0x12d2d2c
      f4: 4b4b4b4b     	blmi	0x12d2e28 <.debug_info+0x12d2e28> @ imm = #0x12d2d2c
      f8: 4b4b4b4c     	blmi	0x12d2e30 <.debug_info+0x12d2e30> @ imm = #0x12d2d30
      fc: 4c4b4b4c     	<unknown>
     100: 024b4c4b     	subeq	r4, r11, #19200
     104: 01010004     	mrseq	r0, apsr

Disassembly of section .debug_line_str:

00000000 <.debug_line_str>:
       0: 6d6f682f     	stclvs	p8, c6, [pc, #-188]!
       4: 656a2f65     	strbvs	r2, [r10, #-0xf65]!
       8: 6e696b6e     	vnmulvs.f64	d22, d9, d30
       c: 67612f73     	<unknown>
      10: 2f746e65     	svchs	#0x746e65
      14: 6b726f77     	blvs	0x1c9bdf8 <.debug_line_str+0x1c9bdf8> @ imm = #0x1c9bddc
      18: 63617073     	<unknown>
      1c: 74612f65     	strbtvc	r2, [r1], #-3941
      20: 322f6566     	eorlo	r6, pc, #427819008
      24: 2f782e30     	svchs	#0x782e30
      28: 6c697562     	stclvs	p5, c7, [r9], #-392
      2c: 74612d64     	strbtvc	r2, [r1], #-3428
      30: 702d6566     	eorvc	r6, sp, r6, ror #10
      34: 616b6361     	<unknown>
      38: 622f6567     	eorvs	r6, pc, #432013312
      3c: 646c6975     	strbtvs	r6, [r12], #-2421
      40: 77656e5f     	<unknown>
      44: 5f62696c     	svcpl	#0x62696c
      48: 7265766f     	rsbvc	r7, r5, #116391936
      4c: 2f79616c     	svchs	#0x79616c
      50: 7065645f     	rsbvc	r6, r5, pc, asr r4
      54: 656e2f73     	strbvs	r2, [lr, #-0xf73]!
      58: 62696c77     	rsbvs	r6, r9, #30464
      5c: 6372732d     	cmnvs	r2, #-1275068416
      60: 77656e2f     	strbvc	r6, [r5, -pc, lsr #28]!
      64: 2f62696c     	svchs	#0x62696c
      68: 6362696c     	<unknown>
      6c: 63616d2f     	<unknown>
      70: 656e6968     	strbvs	r6, [lr, #-0x968]!
      74: 6d72612f     	ldclvs	p1, c6, [r2, #-188]!
      78: 6165612f     	<unknown>
      7c: 6d5f6962     	ldclvs	p9, c6, [pc, #-392]     @ 0xfffffffffffffefc <_end+0xfffffffffc6f86ec>
      80: 65736d65     	ldrbvs	r6, [r3, #-0xd65]!
      84: 6f732d74     	svcvs	#0x732d74
      88: 532e7466     	<unknown>
      8c: 65645f00     	strbvs	r5, [r4, #-0xf00]!
      90: 6e2f7370     	mcrvs	p3, #0x1, r7, c15, c0, #0x3
      94: 696c7765     	stmdbvs	r12!, {r0, r2, r5, r6, r8, r9, r10, r12, sp, lr} ^
      98: 72732d62     	rsbsvc	r2, r3, #6272
      9c: 656e2f63     	strbvs	r2, [lr, #-0xf63]!
      a0: 62696c77     	rsbvs	r6, r9, #30464
      a4: 62696c2f     	rsbvs	r6, r9, #12032
      a8: 616d2f63     	<unknown>
      ac: 6e696863     	cdpvs	p8, #0x6, c6, c9, c3, #0x3
      b0: 72612f65     	rsbvc	r2, r1, #404
      b4: 65612f6d     	strbvs	r2, [r1, #-0xf6d]!
      b8: 5f696261     	svcpl	#0x696261
      bc: 636d656d     	<unknown>
      c0: 632e726c     	<unknown>
      c4: 6f682f00     	svcvs	#0x682f00
      c8: 6a2f656d     	bvs	0xbd9684 <.debug_line_str+0xbd9684> @ imm = #0xbd95b4
      cc: 696b6e65     	stmdbvs	r11!, {r0, r2, r5, r6, r9, r10, r11, sp, lr} ^
      d0: 612f736e     	<unknown>
      d4: 746e6567     	strbtvc	r6, [lr], #-1383
      d8: 726f772f     	rsbvc	r7, pc, #12320768
      dc: 6170736b     	cmnvs	r0, r11, ror #6
      e0: 612f6563     	<unknown>
      e4: 2f656674     	svchs	#0x656674
      e8: 782e3032     	stmdavc	lr!, {r1, r4, r5, r12, sp}
      ec: 6975622f     	ldmdbvs	r5!, {r0, r1, r2, r3, r5, r9, sp, lr} ^
      f0: 612d646c     	<unknown>
      f4: 2d656674     	stclhs	p6, c6, [r5, #-464]!
      f8: 6b636170     	blvs	0x18d86c0 <.debug_line_str+0x18d86c0> @ imm = #0x18d85c0
      fc: 2f656761     	svchs	#0x656761
     100: 6c697562     	stclvs	p5, c7, [r9], #-392
     104: 656e5f64     	strbvs	r5, [lr, #-0xf64]!
     108: 62696c77     	rsbvs	r6, r9, #30464
     10c: 65766f5f     	ldrbvs	r6, [r6, #-0xf5f]!
     110: 79616c72     	stmdbvc	r1!, {r1, r4, r5, r6, r10, r11, sp, lr} ^
     114: 65645f2f     	strbvs	r5, [r4, #-0xf2f]!
     118: 6e2f7370     	mcrvs	p3, #0x1, r7, c15, c0, #0x3
     11c: 696c7765     	stmdbvs	r12!, {r0, r2, r5, r6, r8, r9, r10, r12, sp, lr} ^
     120: 72732d62     	rsbsvc	r2, r3, #6272
     124: 656e2f63     	strbvs	r2, [lr, #-0xf63]!
     128: 62696c77     	rsbvs	r6, r9, #30464
     12c: 62696c2f     	rsbvs	r6, r9, #12032
     130: 616d2f63     	<unknown>
     134: 6e696863     	cdpvs	p8, #0x6, c6, c9, c3, #0x3
     138: 72612f65     	rsbvc	r2, r1, #404
     13c: 65612f6d     	strbvs	r2, [r1, #-0xf6d]!
     140: 5f696261     	svcpl	#0x696261
     144: 636d656d     	<unknown>
     148: 632e726c     	<unknown>
     14c: 6f682f00     	svcvs	#0x682f00
     150: 6a2f656d     	bvs	0xbd970c <.debug_line_str+0xbd970c> @ imm = #0xbd95b4
     154: 696b6e65     	stmdbvs	r11!, {r0, r2, r5, r6, r9, r10, r11, sp, lr} ^
     158: 612f736e     	<unknown>
     15c: 746e6567     	strbtvc	r6, [lr], #-1383
     160: 726f772f     	rsbvc	r7, pc, #12320768
     164: 6170736b     	cmnvs	r0, r11, ror #6
     168: 612f6563     	<unknown>
     16c: 2f656674     	svchs	#0x656674
     170: 782e3032     	stmdavc	lr!, {r1, r4, r5, r12, sp}
     174: 6975622f     	ldmdbvs	r5!, {r0, r1, r2, r3, r5, r9, sp, lr} ^
     178: 612d646c     	<unknown>
     17c: 2d656674     	stclhs	p6, c6, [r5, #-464]!
     180: 6b636170     	blvs	0x18d8748 <.debug_line_str+0x18d8748> @ imm = #0x18d85c0
     184: 2f656761     	svchs	#0x656761
     188: 6c697562     	stclvs	p5, c7, [r9], #-392
     18c: 656e5f64     	strbvs	r5, [lr, #-0xf64]!
     190: 62696c77     	rsbvs	r6, r9, #30464
     194: 65766f5f     	ldrbvs	r6, [r6, #-0xf5f]!
     198: 79616c72     	stmdbvc	r1!, {r1, r4, r5, r6, r10, r11, sp, lr} ^
     19c: 6c756d2f     	ldclvs	p13, c6, [r5], #-188
     1a0: 696c6974     	stmdbvs	r12!, {r2, r4, r5, r6, r8, r11, sp, lr} ^
     1a4: 75622d62     	strbvc	r2, [r2, #-0xd62]!
     1a8: 73646c69     	<unknown>
     1ac: 77656e2f     	strbvc	r6, [r5, -pc, lsr #28]!
     1b0: 2f62696c     	svchs	#0x62696c
     1b4: 766d7261     	strbtvc	r7, [sp], -r1, ror #4
     1b8: 735f6137     	cmpvc	pc, #-1073741811
     1bc: 5f74666f     	svcpl	#0x74666f
     1c0: 76706676     	uhsub16vc	r6, r0, r6
     1c4: 31645f33     	<unknown>
     1c8: 6e755f36     	mrcvs	p15, #0x3, r5, c5, c6, #0x1
     1cc: 67696c61     	strbvs	r6, [r9, -r1, ror #24]!
     1d0: 2f64656e     	svchs	#0x64656e
     1d4: 6c697562     	stclvs	p5, c7, [r9], #-392
     1d8: 72612f64     	rsbvc	r2, r1, #100, #30
     1dc: 6f6e2d6d     	svcvs	#0x6e2d6d
     1e0: 652d656e     	strvs	r6, [sp, #-0x56e]!
     1e4: 2f696261     	svchs	#0x696261
     1e8: 6c77656e     	ldclvs	p5, c6, [r7], #-440
     1ec: 2f006269     	svchs	#0x6269
     1f0: 656d6f68     	strbvs	r6, [sp, #-0xf68]!
     1f4: 6e656a2f     	vmulvs.f32	s13, s10, s31
     1f8: 736e696b     	<unknown>
     1fc: 6567612f     	strbvs	r6, [r7, #-0x12f]!
     200: 772f746e     	strvc	r7, [pc, -lr, ror #8]!
     204: 736b726f     	<unknown>
     208: 65636170     	strbvs	r6, [r3, #-0x170]!
     20c: 6674612f     	ldrbtvs	r6, [r4], -pc, lsr #2
     210: 30322f65     	eorslo	r2, r2, r5, ror #30
     214: 622f782e     	eorvs	r7, pc, #3014656
     218: 646c6975     	strbtvs	r6, [r12], #-2421
     21c: 6674612d     	ldrbtvs	r6, [r4], -sp, lsr #2
     220: 61702d65     	cmnvs	r0, r5, ror #26
     224: 67616b63     	strbvs	r6, [r1, -r3, ror #22]!
     228: 75622f65     	strbvc	r2, [r2, #-0xf65]!
     22c: 5f646c69     	svcpl	#0x646c69
     230: 6c77656e     	ldclvs	p5, c6, [r7], #-440
     234: 6f5f6269     	svcvs	#0x5f6269
     238: 6c726576     	ldclvs	p5, c6, [r2], #-472
     23c: 61007961     	<unknown>
     240: 69626165     	stmdbvs	r2!, {r0, r2, r5, r6, r8, sp, lr} ^
     244: 6d656d5f     	stclvs	p13, c6, [r5, #-380]!
     248: 2d746573     	ldclhs	p5, c6, [r4, #-460]!
     24c: 2e6d7261     	cdphs	p2, #0x6, c7, c13, c1, #0x3
     250: 6c6c0053     	stclvs	p0, c0, [r12], #-332
     254: 6c2f6d76     	stcvs	p13, c6, [pc], #-472
     258: 632f6269     	<unknown>
     25c: 676e616c     	strbvs	r6, [lr, -r12, ror #2]!
     260: 2f30322f     	svchs	#0x30322f
     264: 6c636e69     	stclvs	p14, c6, [r3], #-420
     268: 2f656475     	svchs	#0x656475
     26c: 74735f5f     	ldrbtvc	r5, [r3], #-3935
     270: 66656464     	strbtvs	r6, [r5], -r4, ror #8
     274: 7a69735f     	bvc	0x1a5cff8 <.debug_line_str+0x1a5cff8> @ imm = #0x1a5cd7c
     278: 2e745f65     	cdphs	p15, #0x7, c5, c4, c5, #0x3
     27c: 682f0068     	stmdavs	pc!, {r3, r5, r6}
     280: 2f656d6f     	svchs	#0x656d6f
     284: 6b6e656a     	blvs	0x1b99834 <.debug_line_str+0x1b99834> @ imm = #0x1b995a8
     288: 2f736e69     	svchs	#0x736e69
     28c: 6e656761     	cdpvs	p7, #0x6, c6, c5, c1, #0x3
     290: 6f772f74     	svcvs	#0x772f74
     294: 70736b72     	rsbsvc	r6, r3, r2, ror r11
     298: 2f656361     	svchs	#0x656361
     29c: 65667461     	strbvs	r7, [r6, #-0x461]!
     2a0: 2e30322f     	cdphs	p2, #0x3, c3, c0, c15, #0x1
     2a4: 75622f78     	strbvc	r2, [r2, #-0xf78]!
     2a8: 2d646c69     	stclhs	p12, c6, [r4, #-420]!
     2ac: 65667461     	strbvs	r7, [r6, #-0x461]!
     2b0: 6361702d     	<unknown>
     2b4: 6567616b     	strbvs	r6, [r7, #-0x16b]!
     2b8: 6975622f     	ldmdbvs	r5!, {r0, r1, r2, r3, r5, r9, sp, lr} ^
     2bc: 6e5f646c     	cdpvs	p4, #0x5, c6, c15, c12, #0x3
     2c0: 696c7765     	stmdbvs	r12!, {r0, r2, r5, r6, r8, r9, r10, r12, sp, lr} ^
     2c4: 766f5f62     	strbtvc	r5, [pc], -r2, ror #30
     2c8: 616c7265     	<unknown>
     2cc: 645f2f79     	ldrbvs	r2, [pc], #-3961
     2d0: 2f737065     	svchs	#0x737065
     2d4: 6c77656e     	ldclvs	p5, c6, [r7], #-440
     2d8: 732d6269     	<unknown>
     2dc: 6e2f6372     	mcrvs	p3, #0x1, r6, c15, c2, #0x3
     2e0: 696c7765     	stmdbvs	r12!, {r0, r2, r5, r6, r8, r9, r10, r12, sp, lr} ^
     2e4: 696c2f62     	stmdbvs	r12!, {r1, r5, r6, r8, r9, r10, r11, sp} ^
     2e8: 6d2f6362     	stcvs	p3, c6, [pc, #-392]!
     2ec: 69686361     	stmdbvs	r8!, {r0, r5, r6, r8, r9, sp, lr} ^
     2f0: 612f656e     	<unknown>
     2f4: 72 6d 00     	<unknown>

Disassembly of section .debug_aranges:

00000000 <.debug_aranges>:
       0: 0000001c     	andeq	r0, r0, r12, lsl r0
       4: 007e0002     	rsbseq	r0, lr, r2
       8: 00040000     	andeq	r0, r4, r0
       c: 00000000     	andeq	r0, r0, r0
      10: 038000c0     	orreq	r0, r0, #192
      14: 000000f4     	strdeq	r0, r1, [r0], -r4
		...

Disassembly of section .comment:

00000000 <.comment>:
       0: 6e616c63     	cdpvs	p12, #0x6, c6, c1, c3, #0x3
       4: 65762067     	ldrbvs	r2, [r6, #-0x67]!
       8: 6f697372     	svcvs	#0x697372
       c: 3032206e     	eorslo	r2, r2, lr, rrx
      10: 302e312e     	eorlo	r3, lr, lr, lsr #2
      14: 694c0000     	<unknown>
      18: 72656b6e     	rsbvc	r6, r5, #112640
      1c: 4c4c203a     	mcrrmi	p0, #0x3, r2, r12, c10
      20: 30322044     	eorslo	r2, r2, r4, asr #32
      24: 302e312e     	eorlo	r3, lr, lr, lsr #2
      28: 00           	<unknown>

Disassembly of section .symtab:

00000000 <.symtab>:
		...
      10: 00000001     	andeq	r0, r0, r1
		...
      1c: fff10004     	<unknown>
      20: 00000010     	andeq	r0, r0, r0, lsl r0
      24: 038001b4     	orreq	r0, r0, #180, #2
      28: 00000000     	andeq	r0, r0, r0
      2c: 00010000     	andeq	r0, r1, r0
      30: 00000013     	andeq	r0, r0, r3, lsl r0
      34: 038000c0     	orreq	r0, r0, #192
      38: 00000000     	andeq	r0, r0, r0
      3c: 00010000     	andeq	r0, r1, r0
      40: 00000016     	andeq	r0, r0, r6, lsl r0
		...
      4c: fff10004     	<unknown>
      50: 00000020     	andeq	r0, r0, r0, lsr #32
      54: 03800020     	orreq	r0, r0, #32
      58: 00000000     	andeq	r0, r0, r0
      5c: 00010000     	andeq	r0, r1, r0
      60: 00000023     	andeq	r0, r0, r3, lsr #32
      64: 03800000     	orreq	r0, r0, #0
      68: 00000020     	andeq	r0, r0, r0, lsr #32
      6c: 00010001     	andeq	r0, r1, r1
      70: 00000033     	andeq	r0, r0, r3, lsr r0
      74: 03800000     	orreq	r0, r0, #0
      78: 00000020     	andeq	r0, r0, r0, lsr #32
      7c: 00010001     	andeq	r0, r1, r1
      80: 0000003e     	andeq	r0, r0, lr, lsr r0
      84: 03800020     	orreq	r0, r0, #32
      88: 0000007c     	andeq	r0, r0, r12, ror r0
      8c: 00010012     	andeq	r0, r1, r2, lsl r0
      90: 00000049     	andeq	r0, r0, r9, asr #32
      94: 03804010     	orreq	r4, r0, #16
      98: 00000000     	andeq	r0, r0, r0
      9c: 00160010     	andseq	r0, r6, r0, lsl r0
      a0: 00000055     	andeq	r0, r0, r5, asr r0
      a4: 03804010     	orreq	r4, r0, #16
      a8: 00000000     	andeq	r0, r0, r0
      ac: 00160010     	andseq	r0, r6, r0, lsl r0
      b0: 0000005f     	andeq	r0, r0, pc, asr r0
		...
      bc: 00130010     	andseq	r0, r3, r0, lsl r0
      c0: 0000006c     	andeq	r0, r0, r12, rrx
		...
      cc: 00130010     	andseq	r0, r3, r0, lsl r0
      d0: 00000077     	andeq	r0, r0, r7, ror r0
      d4: 038000c0     	orreq	r0, r0, #192
      d8: 000000f4     	strdeq	r0, r1, [r0], -r4
      dc: 00010012     	andeq	r0, r1, r2, lsl r0
      e0: 00000086     	andeq	r0, r0, r6, lsl #1
      e4: 038000c0     	orreq	r0, r0, #192
      e8: 000000f4     	strdeq	r0, r1, [r0], -r4
      ec: 00010012     	andeq	r0, r1, r2, lsl r0
      f0: 00000096     	muleq	r0, r6, r0
      f4: 038000c0     	orreq	r0, r0, #192
      f8: 000000f4     	strdeq	r0, r1, [r0], -r4
      fc: 00010012     	andeq	r0, r1, r2, lsl r0
     100: 000000a6     	andeq	r0, r0, r6, lsr #1
     104: 038001b4     	orreq	r0, r0, #180, #2
     108: 00000008     	andeq	r0, r0, r8
     10c: 00010012     	andeq	r0, r1, r2, lsl r0
     110: 000000b5     	strheq	r0, [r0], -r5
     114: 038001b4     	orreq	r0, r0, #180, #2
     118: 00000008     	andeq	r0, r0, r8
     11c: 00010012     	andeq	r0, r1, r2, lsl r0
     120: 000000c5     	andeq	r0, r0, r5, asr #1
     124: 038001b4     	orreq	r0, r0, #180, #2
     128: 00000008     	andeq	r0, r0, r8
     12c: 00010012     	andeq	r0, r1, r2, lsl r0
     130: 000000d5     	ldrdeq	r0, r1, [r0], -r5
     134: 00002000     	andeq	r2, r0, r0
     138: 00000000     	andeq	r0, r0, r0
     13c: fff10010     	<unknown>
     140: 000000e1     	andeq	r0, r0, r1, ror #1
     144: 00100000     	andseq	r0, r0, r0
     148: 00000000     	andeq	r0, r0, r0
     14c: fff10010     	<unknown>
     150: 000000ec     	andeq	r0, r0, r12, ror #1
     154: 00000400     	andeq	r0, r0, r0, lsl #8
     158: 00000000     	andeq	r0, r0, r0
     15c: fff10010     	<unknown>
     160: 000000fe     	strdeq	r0, r1, [r0], -lr
     164: 00000800     	andeq	r0, r0, r0, lsl #16
     168: 00000000     	andeq	r0, r0, r0
     16c: fff10010     	<unknown>
     170: 00000115     	andeq	r0, r0, r5, lsl r1
     174: 00000400     	andeq	r0, r0, r0, lsl #8
     178: 00000000     	andeq	r0, r0, r0
     17c: fff10010     	<unknown>
     180: 00000125     	andeq	r0, r0, r5, lsr #2
     184: 00000400     	andeq	r0, r0, r0, lsl #8
     188: 00000000     	andeq	r0, r0, r0
     18c: fff10010     	<unknown>
     190: 00000135     	andeq	r0, r0, r5, lsr r1
     194: 00000400     	andeq	r0, r0, r0, lsl #8
     198: 00000000     	andeq	r0, r0, r0
     19c: fff10010     	<unknown>
     1a0: 00000147     	andeq	r0, r0, r7, asr #2
		...
     1ac: 00120010     	andseq	r0, r2, r0, lsl r0
     1b0: 00000155     	andeq	r0, r0, r5, asr r1
     1b4: 038001bc     	orreq	r0, r0, #188, #2
     1b8: 00000000     	andeq	r0, r0, r0
     1bc: 00040010     	andeq	r0, r4, r0, lsl r0
     1c0: 00000164     	andeq	r0, r0, r4, ror #2
     1c4: 038001bc     	orreq	r0, r0, #188, #2
     1c8: 00000000     	andeq	r0, r0, r0
     1cc: 00050010     	andeq	r0, r5, r0, lsl r0
     1d0: 00000170     	andeq	r0, r0, r0, ror r1
     1d4: 03906010     	orrseq	r6, r0, #16
     1d8: 00000000     	andeq	r0, r0, r0
     1dc: 00180010     	andseq	r0, r8, r0, lsl r0
     1e0: 00000177     	andeq	r0, r0, r7, ror r1
     1e4: 038001bc     	orreq	r0, r0, #188, #2
     1e8: 00000000     	andeq	r0, r0, r0
     1ec: 00020010     	andeq	r0, r2, r0, lsl r0
     1f0: 00000186     	andeq	r0, r0, r6, lsl #3
     1f4: 038001bc     	orreq	r0, r0, #188, #2
     1f8: 00000000     	andeq	r0, r0, r0
     1fc: 00020010     	andeq	r0, r2, r0, lsl r0
     200: 00000193     	muleq	r0, r3, r1
     204: 038001bc     	orreq	r0, r0, #188, #2
     208: 00000000     	andeq	r0, r0, r0
     20c: 00030010     	andeq	r0, r3, r0, lsl r0
     210: 000001a3     	andeq	r0, r0, r3, lsr #3
     214: 038001bc     	orreq	r0, r0, #188, #2
     218: 00000000     	andeq	r0, r0, r0
     21c: 00030010     	andeq	r0, r3, r0, lsl r0
     220: 000001b1     	strheq	r0, [r0], -r1
     224: 038001bc     	orreq	r0, r0, #188, #2
     228: 00000000     	andeq	r0, r0, r0
     22c: 00040010     	andeq	r0, r4, r0, lsl r0
     230: 000001be     	strheq	r0, [r0], -lr
     234: 038001bc     	orreq	r0, r0, #188, #2
     238: 00000000     	andeq	r0, r0, r0
     23c: 00050010     	andeq	r0, r5, r0, lsl r0
     240: 000001cc     	andeq	r0, r0, r12, asr #3
     244: 038001bc     	orreq	r0, r0, #188, #2
     248: 00000000     	andeq	r0, r0, r0
     24c: 00060010     	andeq	r0, r6, r0, lsl r0
     250: 000001d9     	ldrdeq	r0, r1, [r0], -r9
     254: 038001bc     	orreq	r0, r0, #188, #2
     258: 00000000     	andeq	r0, r0, r0
     25c: 00060010     	andeq	r0, r6, r0, lsl r0
     260: 000001e4     	andeq	r0, r0, r4, ror #3
     264: 038001bc     	orreq	r0, r0, #188, #2
     268: 00000000     	andeq	r0, r0, r0
     26c: 00070010     	andeq	r0, r7, r0, lsl r0
     270: 000001f2     	strdeq	r0, r1, [r0], -r2
     274: 038001bc     	orreq	r0, r0, #188, #2
     278: 00000000     	andeq	r0, r0, r0
     27c: 00070010     	andeq	r0, r7, r0, lsl r0
     280: 000001fe     	strdeq	r0, r1, [r0], -lr
     284: 038001bc     	orreq	r0, r0, #188, #2
     288: 00000000     	andeq	r0, r0, r0
     28c: 00080010     	andeq	r0, r8, r0, lsl r0
     290: 0000020c     	andeq	r0, r0, r12, lsl #4
     294: 038001bc     	orreq	r0, r0, #188, #2
     298: 00000000     	andeq	r0, r0, r0
     29c: 00080010     	andeq	r0, r8, r0, lsl r0
     2a0: 0000021d     	andeq	r0, r0, sp, lsl r2
     2a4: 038001bc     	orreq	r0, r0, #188, #2
     2a8: 00000000     	andeq	r0, r0, r0
     2ac: 00080010     	andeq	r0, r8, r0, lsl r0
     2b0: 0000022a     	andeq	r0, r0, r10, lsr #4
     2b4: 038001bc     	orreq	r0, r0, #188, #2
     2b8: 00000000     	andeq	r0, r0, r0
     2bc: 00080010     	andeq	r0, r8, r0, lsl r0
     2c0: 0000023a     	andeq	r0, r0, r10, lsr r2
     2c4: 038001bc     	orreq	r0, r0, #188, #2
     2c8: 00000000     	andeq	r0, r0, r0
     2cc: 00090010     	andeq	r0, r9, r0, lsl r0
     2d0: 00000248     	andeq	r0, r0, r8, asr #4
     2d4: 038001bc     	orreq	r0, r0, #188, #2
     2d8: 00000000     	andeq	r0, r0, r0
     2dc: 00090010     	andeq	r0, r9, r0, lsl r0
     2e0: 00000259     	andeq	r0, r0, r9, asr r2
     2e4: 038001bc     	orreq	r0, r0, #188, #2
     2e8: 00000000     	andeq	r0, r0, r0
     2ec: 00090010     	andeq	r0, r9, r0, lsl r0
     2f0: 00000266     	andeq	r0, r0, r6, ror #4
     2f4: 038001bc     	orreq	r0, r0, #188, #2
     2f8: 00000000     	andeq	r0, r0, r0
     2fc: 00090010     	andeq	r0, r9, r0, lsl r0
     300: 00000276     	andeq	r0, r0, r6, ror r2
     304: 038001bc     	orreq	r0, r0, #188, #2
     308: 00000000     	andeq	r0, r0, r0
     30c: 000a0010     	andeq	r0, r10, r0, lsl r0
     310: 00000284     	andeq	r0, r0, r4, lsl #5
     314: 038001bc     	orreq	r0, r0, #188, #2
     318: 00000000     	andeq	r0, r0, r0
     31c: 000a0010     	andeq	r0, r10, r0, lsl r0
     320: 00000290     	muleq	r0, r0, r2
     324: 038001bc     	orreq	r0, r0, #188, #2
     328: 00000000     	andeq	r0, r0, r0
     32c: 000b0010     	andeq	r0, r11, r0, lsl r0
     330: 000002a4     	andeq	r0, r0, r4, lsr #5
     334: 038001bc     	orreq	r0, r0, #188, #2
     338: 00000000     	andeq	r0, r0, r0
     33c: 000b0010     	andeq	r0, r11, r0, lsl r0
     340: 000002b6     	strheq	r0, [r0], -r6
     344: 03804000     	orreq	r4, r0, #0
     348: 00000000     	andeq	r0, r0, r0
     34c: 000c0010     	andeq	r0, r12, r0, lsl r0
     350: 000002c6     	andeq	r0, r0, r6, asr #5
     354: 03804000     	orreq	r4, r0, #0
     358: 00000000     	andeq	r0, r0, r0
     35c: 000c0010     	andeq	r0, r12, r0, lsl r0
     360: 000002d4     	ldrdeq	r0, r1, [r0], -r4
     364: 03804000     	orreq	r4, r0, #0
     368: 00000000     	andeq	r0, r0, r0
     36c: 000d0010     	andeq	r0, sp, r0, lsl r0
     370: 000002e2     	andeq	r0, r0, r2, ror #5
     374: 03804010     	orreq	r4, r0, #16
     378: 00000000     	andeq	r0, r0, r0
     37c: 000d0010     	andeq	r0, sp, r0, lsl r0
     380: 000002ee     	andeq	r0, r0, lr, ror #5
     384: 03804010     	orreq	r4, r0, #16
     388: 00000000     	andeq	r0, r0, r0
     38c: 000e0010     	andeq	r0, lr, r0, lsl r0
     390: 00000304     	andeq	r0, r0, r4, lsl #6
     394: 03804010     	orreq	r4, r0, #16
     398: 00000000     	andeq	r0, r0, r0
     39c: 000e0010     	andeq	r0, lr, r0, lsl r0
     3a0: 00000318     	andeq	r0, r0, r8, lsl r3
     3a4: 03804010     	orreq	r4, r0, #16
     3a8: 00000000     	andeq	r0, r0, r0
     3ac: 000f0010     	andeq	r0, pc, r0, lsl r0
     3b0: 0000032b     	andeq	r0, r0, r11, lsr #6
     3b4: 03804010     	orreq	r4, r0, #16
     3b8: 00000000     	andeq	r0, r0, r0
     3bc: 000f0010     	andeq	r0, pc, r0, lsl r0
     3c0: 0000033c     	andeq	r0, r0, r12, lsr r3
     3c4: 03804010     	orreq	r4, r0, #16
     3c8: 00000000     	andeq	r0, r0, r0
     3cc: 00100010     	andseq	r0, r0, r0, lsl r0
     3d0: 0000034f     	andeq	r0, r0, pc, asr #6
     3d4: 03804010     	orreq	r4, r0, #16
     3d8: 00000000     	andeq	r0, r0, r0
     3dc: 00100010     	andseq	r0, r0, r0, lsl r0
     3e0: 00000360     	andeq	r0, r0, r0, ror #6
		...
     3ec: 00110010     	andseq	r0, r1, r0, lsl r0
     3f0: 00000377     	andeq	r0, r0, r7, ror r3
     3f4: 0000003a     	andeq	r0, r0, r10, lsr r0
     3f8: 00000000     	andeq	r0, r0, r0
     3fc: 00110010     	andseq	r0, r1, r0, lsl r0
     400: 0000038c     	andeq	r0, r0, r12, lsl #7
		...
     40c: 00120010     	andseq	r0, r2, r0, lsl r0
     410: 00000398     	muleq	r0, r8, r3
		...
     41c: 00140010     	andseq	r0, r4, r0, lsl r0
     420: 000003a6     	andeq	r0, r0, r6, lsr #7
		...
     42c: 00140010     	andseq	r0, r4, r0, lsl r0
     430: 000003b2     	strheq	r0, [r0], -r2
		...
     43c: 00150010     	andseq	r0, r5, r0, lsl r0
     440: 000003bf     	strheq	r0, [r0], -pc
		...
     44c: 00150010     	andseq	r0, r5, r0, lsl r0
     450: 000003ca     	andeq	r0, r0, r10, asr #7
		...
     45c: 00120010     	andseq	r0, r2, r0, lsl r0
     460: 000003d5     	ldrdeq	r0, r1, [r0], -r5
     464: 038001bc     	orreq	r0, r0, #188, #2
     468: 00000000     	andeq	r0, r0, r0
     46c: 00040010     	andeq	r0, r4, r0, lsl r0
     470: 000003e1     	andeq	r0, r0, r1, ror #7
     474: 03804010     	orreq	r4, r0, #16
     478: 00000000     	andeq	r0, r0, r0
     47c: 00170010     	andseq	r0, r7, r0, lsl r0
     480: 000003e7     	andeq	r0, r0, r7, ror #7
     484: 03804010     	orreq	r4, r0, #16
     488: 00000000     	andeq	r0, r0, r0
     48c: 00170010     	andseq	r0, r7, r0, lsl r0
     490: 000003f0     	strdeq	r0, r1, [r0], -r0
     494: 03804010     	orreq	r4, r0, #16
     498: 00000000     	andeq	r0, r0, r0
     49c: 00170010     	andseq	r0, r7, r0, lsl r0
     4a0: 000003fc     	strdeq	r0, r1, [r0], -r12
     4a4: 03904010     	orrseq	r4, r0, #16
     4a8: 00000000     	andeq	r0, r0, r0
     4ac: 00170010     	andseq	r0, r7, r0, lsl r0
     4b0: 00000406     	andeq	r0, r0, r6, lsl #8
     4b4: 03904010     	orrseq	r4, r0, #16
     4b8: 00000000     	andeq	r0, r0, r0
     4bc: 00170010     	andseq	r0, r7, r0, lsl r0
     4c0: 00000410     	andeq	r0, r0, r0, lsl r4
     4c4: 03904010     	orrseq	r4, r0, #16
     4c8: 00000000     	andeq	r0, r0, r0
     4cc: 00180010     	andseq	r0, r8, r0, lsl r0
     4d0: 0000041b     	andeq	r0, r0, r11, lsl r4
     4d4: 03906010     	orrseq	r6, r0, #16
     4d8: 00000000     	andeq	r0, r0, r0
     4dc: 00180010     	andseq	r0, r8, r0, lsl r0
     4e0: 00000423     	andeq	r0, r0, r3, lsr #8
     4e4: 03906010     	orrseq	r6, r0, #16
     4e8: 00000000     	andeq	r0, r0, r0
     4ec: 00180010     	andseq	r0, r8, r0, lsl r0
     4f0: 00000432     	andeq	r0, r0, r2, lsr r4
     4f4: 03906410     	orrseq	r6, r0, #16, #8
     4f8: 00000000     	andeq	r0, r0, r0
     4fc: 00180010     	andseq	r0, r8, r0, lsl r0
     500: 0000043e     	andeq	r0, r0, lr, lsr r4
     504: 03906410     	orrseq	r6, r0, #16, #8
     508: 00000000     	andeq	r0, r0, r0
     50c: 00180010     	andseq	r0, r8, r0, lsl r0
     510: 00000454     	andeq	r0, r0, r4, asr r4
     514: 03906c10     	orrseq	r6, r0, #16, #24
     518: 00000000     	andeq	r0, r0, r0
     51c: 00180010     	andseq	r0, r8, r0, lsl r0
     520: 00000467     	andeq	r0, r0, r7, ror #8
     524: 03906c10     	orrseq	r6, r0, #16, #24
     528: 00000000     	andeq	r0, r0, r0
     52c: 00180010     	andseq	r0, r8, r0, lsl r0
     530: 00000478     	andeq	r0, r0, r8, ror r4
     534: 03907010     	orrseq	r7, r0, #16
     538: 00000000     	andeq	r0, r0, r0
     53c: 00180010     	andseq	r0, r8, r0, lsl r0
     540: 00000486     	andeq	r0, r0, r6, lsl #9
     544: 03907010     	orrseq	r7, r0, #16
     548: 00000000     	andeq	r0, r0, r0
     54c: 00180010     	andseq	r0, r8, r0, lsl r0
     550: 00000495     	muleq	r0, r5, r4
     554: 03907410     	orrseq	r7, r0, #16, #8
     558: 00000000     	andeq	r0, r0, r0
     55c: 00180010     	andseq	r0, r8, r0, lsl r0
     560: 000004a1     	andeq	r0, r0, r1, lsr #9
     564: 03907410     	orrseq	r7, r0, #16, #8
     568: 00000000     	andeq	r0, r0, r0
     56c: 00180010     	andseq	r0, r8, r0, lsl r0
     570: 000004b2     	strheq	r0, [r0], -r2
     574: 03907810     	orrseq	r7, r0, #16, #16
     578: 00000000     	andeq	r0, r0, r0
     57c: 00180010     	andseq	r0, r8, r0, lsl r0
     580: 000004c0     	andeq	r0, r0, r0, asr #9
     584: 03907810     	orrseq	r7, r0, #16, #16
     588: 00000000     	andeq	r0, r0, r0
     58c: 00180010     	andseq	r0, r8, r0, lsl r0

Disassembly of section .shstrtab:

00000000 <.shstrtab>:
       0: 65742e00     	ldrbvs	r2, [r4, #-0xe00]!
       4: 2e007478     	mcrhs	p4, #0x0, r7, c0, c8, #0x3
       8: 61646f72     	<unknown>
       c: 2e006174     	mcrhs	p1, #0x0, r6, c0, c4, #0x3
      10: 61646f72     	<unknown>
      14: 00316174     	eorseq	r6, r1, r4, ror r1
      18: 6164732e     	<unknown>
      1c: 00326174     	eorseq	r6, r2, r4, ror r1
      20: 7362732e     	<unknown>
      24: 2e003273     	mcrhs	p2, #0x0, r3, c0, c3, #0x3
      28: 61746164     	cmnvs	r4, r4, ror #2
      2c: 61642e00     	<unknown>
      30: 00316174     	eorseq	r6, r1, r4, ror r1
      34: 6f74632e     	svcvs	#0x74632e
      38: 2e007372     	mcrhs	p3, #0x0, r7, c0, c2, #0x3
      3c: 726f7464     	rsbvc	r7, pc, #100, #8
      40: 662e0073     	qsub16vs	r0, lr, r3
      44: 70757869     	rsbsvc	r7, r5, r9, ror #16
      48: 68652e00     	stmdavs	r5!, {r9, r10, r11, sp} ^
      4c: 6172665f     	cmnvs	r2, pc, asr r6
      50: 6468656d     	strbtvs	r6, [r8], #-1389
      54: 6d2e0072     	stcvs	p0, c0, [lr, #-456]!
      58: 745f756d     	ldrbvc	r7, [pc], #-1389
      5c: 2e006c62     	cdphs	p12, #0x0, c6, c0, c2, #0x3
      60: 2e4d5241     	cdphs	p2, #0x4, c5, c13, c1, #0x2
      64: 64697865     	strbtvs	r7, [r9], #-2149
      68: 702e0078     	eorvc	r0, lr, r8, ror r0
      6c: 6e696572     	mcrvs	p5, #0x3, r6, c9, c2, #0x3
      70: 615f7469     	cmpvs	pc, r9, ror #8
      74: 79617272     	stmdbvc	r1!, {r1, r4, r5, r6, r9, r12, sp, lr} ^
      78: 6e692e00     	cdpvs	p14, #0x6, c2, c9, c0, #0x0
      7c: 615f7469     	cmpvs	pc, r9, ror #8
      80: 79617272     	stmdbvc	r1!, {r1, r4, r5, r6, r9, r12, sp, lr} ^
      84: 69662e00     	stmdbvs	r6!, {r9, r10, r11, sp} ^
      88: 615f696e     	cmpvs	pc, lr, ror #18
      8c: 79617272     	stmdbvc	r1!, {r1, r4, r5, r6, r9, r12, sp, lr} ^
      90: 52412e00     	subpl	r2, r1, #0, #28
      94: 74612e4d     	strbtvc	r2, [r1], #-3661
      98: 62697274     	rsbvs	r7, r9, #116, #4
      9c: 73657475     	<unknown>
      a0: 64732e00     	ldrbtvs	r2, [r3], #-3584
      a4: 00617461     	rsbeq	r7, r1, r1, ror #8
      a8: 7362732e     	<unknown>
      ac: 742e0073     	strtvc	r0, [lr], #-115
      b0: 61746164     	cmnvs	r4, r4, ror #2
      b4: 62742e00     	rsbsvs	r2, r4, #0, #28
      b8: 2e007373     	mcrhs	p3, #0x0, r7, c0, c3, #0x3
      bc: 00737362     	rsbseq	r7, r3, r2, ror #6
      c0: 6165682e     	<unknown>
      c4: 732e0070     	<unknown>
      c8: 6b636174     	blvs	0x18d86a0 <.symtab+0x18d86a0> @ imm = #0x18d85d0
      cc: 65642e00     	strbvs	r2, [r4, #-0xe00]!
      d0: 5f677562     	svcpl	#0x677562
      d4: 72626261     	rsbvc	r6, r2, #268435462
      d8: 2e007665     	cdphs	p6, #0x0, c7, c0, c5, #0x3
      dc: 75626564     	strbvc	r6, [r2, #-0x564]!
      e0: 6e695f67     	cdpvs	p15, #0x6, c5, c9, c7, #0x3
      e4: 2e006f66     	cdphs	p15, #0x0, c6, c0, c6, #0x3
      e8: 75626564     	strbvc	r6, [r2, #-0x564]!
      ec: 74735f67     	ldrbtvc	r5, [r3], #-3943
      f0: 666f5f72     	uqsub16vs	r5, pc, r2
      f4: 74657366     	strbtvc	r7, [r5], #-870
      f8: 642e0073     	strtvs	r0, [lr], #-115
      fc: 67756265     	ldrbvs	r6, [r5, -r5, ror #4]!
     100: 7274735f     	rsbsvc	r7, r4, #2080374785
     104: 65642e00     	strbvs	r2, [r4, #-0xe00]!
     108: 5f677562     	svcpl	#0x677562
     10c: 72646461     	rsbvc	r6, r4, #1627389952
     110: 65642e00     	strbvs	r2, [r4, #-0xe00]!
     114: 5f677562     	svcpl	#0x677562
     118: 6d617266     	stclvs	p2, c7, [r1, #-408]!
     11c: 642e0065     	strtvs	r0, [lr], #-101
     120: 67756265     	ldrbvs	r6, [r5, -r5, ror #4]!
     124: 6e696c5f     	mcrvs	p12, #0x3, r6, c9, c15, #0x2
     128: 642e0065     	strtvs	r0, [lr], #-101
     12c: 67756265     	ldrbvs	r6, [r5, -r5, ror #4]!
     130: 6e696c5f     	mcrvs	p12, #0x3, r6, c9, c15, #0x2
     134: 74735f65     	ldrbtvc	r5, [r3], #-3941
     138: 642e0072     	strtvs	r0, [lr], #-114
     13c: 67756265     	ldrbvs	r6, [r5, -r5, ror #4]!
     140: 6172615f     	cmnvs	r2, pc, asr r1
     144: 7365676e     	<unknown>
     148: 6f632e00     	svcvs	#0x632e00
     14c: 6e656d6d     	cdpvs	p13, #0x6, c6, c5, c13, #0x3
     150: 732e0074     	<unknown>
     154: 61746d79     	cmnvs	r4, r9, ror sp
     158: 732e0062     	<unknown>
     15c: 72747368     	rsbsvc	r7, r4, #104, #6
     160: 00626174     	rsbeq	r6, r2, r4, ror r1
     164: 7274732e     	rsbsvc	r7, r4, #-1207959552
     168: 00626174     	rsbeq	r6, r2, r4, ror r1

Disassembly of section .strtab:

00000000 <.strtab>:
       0: 61656100     	<unknown>
       4: 6d5f6962     	ldclvs	p9, c6, [pc, #-392]     @ 0xfffffffffffffe84 <_end+0xfffffffffc6f8674>
       8: 6c636d65     	stclvs	p13, c6, [r3], #-404
       c: 00632e72     	rsbeq	r2, r3, r2, ror lr
      10: 24006124     	strhs	r6, [r0], #-292
      14: 646c0061     	strbtvs	r0, [r12], #-97
      18: 6d65742d     	stclvs	p4, c7, [r5, #-180]!
      1c: 006f2e70     	rsbeq	r2, pc, r0, ror lr
      20: 5f006124     	svcpl	#0x6124
      24: 30314c5a     	eorslo	r4, r1, r10, asr r12
      28: 43786576     	cmnmi	r8, #494927872
      2c: 5365646f     	<unknown>
      30: 76006769     	strvc	r6, [r0], -r9, ror #14
      34: 6f437865     	svcvs	#0x437865
      38: 69536564     	ldmdbvs	r3, {r2, r5, r6, r8, r10, sp, lr} ^
      3c: 65760067     	ldrbvs	r0, [r6, #-0x67]!
      40: 61745378     	cmnvs	r4, r8, ror r3
      44: 70757472     	rsbsvc	r7, r5, r2, ror r4
      48: 625f5f00     	subsvs	r5, pc, #0, #30
      4c: 735f7373     	cmpvc	pc, #-872415231
      50: 74726174     	ldrbtvc	r6, [r2], #-372
      54: 625f5f00     	subsvs	r5, pc, #0, #30
      58: 655f7373     	ldrbvs	r7, [pc, #-0x373]       @ 0xfffffffffffffced <_end+0xfffffffffc6f84dd>
      5c: 5f00646e     	svcpl	#0x646e
      60: 7362735f     	<unknown>
      64: 74735f73     	ldrbtvc	r5, [r3], #-3955
      68: 00747261     	rsbseq	r7, r4, r1, ror #4
      6c: 62735f5f     	rsbsvs	r5, r3, #380
      70: 655f7373     	ldrbvs	r7, [pc, #-0x373]       @ 0xfffffffffffffd05 <_end+0xfffffffffc6f84f5>
      74: 5f00646e     	svcpl	#0x646e
      78: 6165615f     	qdsubvs	r6, pc, r5
      7c: 6d5f6962     	ldclvs	p9, c6, [pc, #-392]     @ 0xfffffffffffffefc <_end+0xfffffffffc6f86ec>
      80: 65736d65     	ldrbvs	r6, [r3, #-0xd65]!
      84: 5f5f0074     	svcpl	#0x5f0074
      88: 62616561     	rsbvs	r6, r1, #406847488
      8c: 656d5f69     	strbvs	r5, [sp, #-0xf69]!
      90: 7465736d     	strbtvc	r7, [r5], #-877
      94: 5f5f0034     	svcpl	#0x5f0034
      98: 62616561     	rsbvs	r6, r1, #406847488
      9c: 656d5f69     	strbvs	r5, [sp, #-0xf69]!
      a0: 7465736d     	strbtvc	r7, [r5], #-877
      a4: 5f5f0038     	svcpl	#0x5f0038
      a8: 62616561     	rsbvs	r6, r1, #406847488
      ac: 656d5f69     	strbvs	r5, [sp, #-0xf69]!
      b0: 726c636d     	rsbvc	r6, r12, #-1275068415
      b4: 615f5f00     	cmpvs	pc, r0, lsl #30
      b8: 69626165     	stmdbvs	r2!, {r0, r2, r5, r6, r8, sp, lr} ^
      bc: 6d656d5f     	stclvs	p13, c6, [r5, #-380]!
      c0: 34726c63     	ldrbtlo	r6, [r2], #-3171
      c4: 615f5f00     	cmpvs	pc, r0, lsl #30
      c8: 69626165     	stmdbvs	r2!, {r0, r2, r5, r6, r8, sp, lr} ^
      cc: 6d656d5f     	stclvs	p13, c6, [r5, #-380]!
      d0: 38726c63     	ldmdalo	r2!, {r0, r1, r5, r6, r10, r11, sp, lr} ^
      d4: 54535f00     	ldrbpl	r5, [r3], #-3840
      d8: 5f4b4341     	svcpl	#0x4b4341
      dc: 455a4953     	ldrbmi	r4, [r10, #-0x953]
      e0: 45485f00     	strbmi	r5, [r8, #-0xf00]
      e4: 535f5041     	cmppl	pc, #65
      e8: 00455a49     	subeq	r5, r5, r9, asr #20
      ec: 4f42415f     	svcmi	#0x42415f
      f0: 535f5452     	cmppl	pc, #1375731712
      f4: 4b434154     	blmi	0x10d064c <.symtab+0x10d064c> @ imm = #0x10d0550
      f8: 5a49535f     	bpl	0x1254e7c <.symtab+0x1254e7c> @ imm = #0x1254d7c
      fc: 535f0045     	cmppl	pc, #69
     100: 52455055     	subpl	r5, r5, #85
     104: 4f534956     	svcmi	#0x534956
     108: 54535f52     	ldrbpl	r5, [r3], #-3922
     10c: 5f4b4341     	svcpl	#0x4b4341
     110: 455a4953     	ldrbmi	r4, [r10, #-0x953]
     114: 52495f00     	subpl	r5, r9, #0, #30
     118: 54535f51     	ldrbpl	r5, [r3], #-3921
     11c: 5f4b4341     	svcpl	#0x4b4341
     120: 455a4953     	ldrbmi	r4, [r10, #-0x953]
     124: 49465f00     	stmdbmi	r6, {r8, r9, r10, r11, r12, lr} ^
     128: 54535f51     	ldrbpl	r5, [r3], #-3921
     12c: 5f4b4341     	svcpl	#0x4b4341
     130: 455a4953     	ldrbmi	r4, [r10, #-0x953]
     134: 4e555f00     	cdpmi	p15, #0x5, c5, c5, c0, #0x0
     138: 5f464544     	svcpl	#0x464544
     13c: 43415453     	movtmi	r5, #0x1453
     140: 49535f4b     	ldmdbmi	r3, {r0, r1, r3, r6, r8, r9, r10, r11, r12, lr} ^
     144: 5f00455a     	svcpl	#0x455a
     148: 6164735f     	qdsubvs	r7, pc, r4
     14c: 735f6174     	cmpvc	pc, #116, #2
     150: 74726174     	ldrbtvc	r6, [r2], #-372
     154: 735f5f00     	cmpvc	pc, #0, #30
     158: 61746164     	cmnvs	r4, r4, ror #2
     15c: 74735f32     	ldrbtvc	r5, [r3], #-3890
     160: 00747261     	rsbseq	r7, r4, r1, ror #4
     164: 62735f5f     	rsbsvs	r5, r3, #380
     168: 5f327373     	svcpl	#0x327373
     16c: 00646e65     	rsbeq	r6, r4, r5, ror #28
     170: 6174735f     	cmnvs	r4, pc, asr r3
     174: 5f006b63     	svcpl	#0x6b63
     178: 646f725f     	strbtvs	r7, [pc], #-607
     17c: 5f617461     	svcpl	#0x617461
     180: 72617473     	rsbvc	r7, r1, #1929379840
     184: 5f5f0074     	svcpl	#0x5f0074
     188: 61646f72     	<unknown>
     18c: 655f6174     	ldrbvs	r6, [pc, #-0x174]       @ 0x20 <.symtab+0x20>
     190: 5f00646e     	svcpl	#0x646e
     194: 646f725f     	strbtvs	r7, [pc], #-607
     198: 31617461     	<unknown>
     19c: 6174735f     	cmnvs	r4, pc, asr r3
     1a0: 5f007472     	svcpl	#0x7472
     1a4: 646f725f     	strbtvs	r7, [pc], #-607
     1a8: 31617461     	<unknown>
     1ac: 646e655f     	strbtvs	r6, [lr], #-1375
     1b0: 735f5f00     	cmpvc	pc, #0, #30
     1b4: 61746164     	cmnvs	r4, r4, ror #2
     1b8: 6e655f32     	mcrvs	p15, #0x3, r5, c5, c2, #0x1
     1bc: 5f5f0064     	svcpl	#0x5f0064
     1c0: 73736273     	cmnvc	r3, #805306375
     1c4: 74735f32     	ldrbtvc	r5, [r3], #-3890
     1c8: 00747261     	rsbseq	r7, r4, r1, ror #4
     1cc: 61645f5f     	qdsubvs	r5, pc, r4
     1d0: 735f6174     	cmpvc	pc, #116, #2
     1d4: 74726174     	ldrbtvc	r6, [r2], #-372
     1d8: 645f5f00     	ldrbvs	r5, [pc], #-3840
     1dc: 5f617461     	svcpl	#0x617461
     1e0: 00646e65     	rsbeq	r6, r4, r5, ror #28
     1e4: 61645f5f     	qdsubvs	r5, pc, r4
     1e8: 5f316174     	svcpl	#0x316174
     1ec: 72617473     	rsbvc	r7, r1, #1929379840
     1f0: 5f5f0074     	svcpl	#0x5f0074
     1f4: 61746164     	cmnvs	r4, r4, ror #2
     1f8: 6e655f31     	mcrvs	p15, #0x3, r5, c5, c1, #0x1
     1fc: 5f5f0064     	svcpl	#0x5f0064
     200: 524f5443     	subpl	r5, pc, #1124073472
     204: 53494c5f     	movtpl	r4, #0x9c5f
     208: 005f5f54     	subseq	r5, pc, r4, asr pc
     20c: 435f5f5f     	cmpmi	pc, #380
     210: 53524f54     	cmppl	r2, #84, #30
     214: 53494c5f     	movtpl	r4, #0x9c5f
     218: 5f5f5f54     	svcpl	#0x5f5f54
     21c: 435f5f00     	cmpmi	pc, #0, #30
     220: 5f524f54     	svcpl	#0x524f54
     224: 5f444e45     	svcpl	#0x444e45
     228: 5f5f005f     	svcpl	#0x5f005f
     22c: 4f54435f     	svcmi	#0x54435f
     230: 455f5352     	ldrbmi	r5, [pc, #-0x352]       @ 0xfffffffffffffee6 <_end+0xfffffffffc6f86d6>
     234: 5f5f444e     	svcpl	#0x5f444e
     238: 5f5f005f     	svcpl	#0x5f005f
     23c: 524f5444     	subpl	r5, pc, #68, #8
     240: 53494c5f     	movtpl	r4, #0x9c5f
     244: 005f5f54     	subseq	r5, pc, r4, asr pc
     248: 445f5f5f     	ldrbmi	r5, [pc], #-3935
     24c: 53524f54     	cmppl	r2, #84, #30
     250: 53494c5f     	movtpl	r4, #0x9c5f
     254: 5f5f5f54     	svcpl	#0x5f5f54
     258: 445f5f00     	ldrbmi	r5, [pc], #-3840
     25c: 5f524f54     	svcpl	#0x524f54
     260: 5f444e45     	svcpl	#0x444e45
     264: 5f5f005f     	svcpl	#0x5f005f
     268: 4f54445f     	svcmi	#0x54445f
     26c: 455f5352     	ldrbmi	r5, [pc, #-0x352]       @ 0xffffffffffffff22 <_end+0xfffffffffc6f8712>
     270: 5f5f444e     	svcpl	#0x5f444e
     274: 5f5f005f     	svcpl	#0x5f005f
     278: 75786966     	ldrbvc	r6, [r8, #-0x966]!
     27c: 74735f70     	ldrbtvc	r5, [r3], #-3952
     280: 00747261     	rsbseq	r7, r4, r1, ror #4
     284: 69665f5f     	stmdbvs	r6!, {r0, r1, r2, r3, r4, r6, r8, r9, r10, r11, r12, lr} ^
     288: 5f707578     	svcpl	#0x707578
     28c: 00646e65     	rsbeq	r6, r4, r5, ror #28
     290: 68655f5f     	stmdavs	r5!, {r0, r1, r2, r3, r4, r6, r8, r9, r10, r11, r12, lr} ^
     294: 6172665f     	cmnvs	r2, pc, asr r6
     298: 6468656d     	strbtvs	r6, [r8], #-1389
     29c: 74735f72     	ldrbtvc	r5, [r3], #-3954
     2a0: 00747261     	rsbseq	r7, r4, r1, ror #4
     2a4: 68655f5f     	stmdavs	r5!, {r0, r1, r2, r3, r4, r6, r8, r9, r10, r11, r12, lr} ^
     2a8: 6172665f     	cmnvs	r2, pc, asr r6
     2ac: 6468656d     	strbtvs	r6, [r8], #-1389
     2b0: 6e655f72     	mcrvs	p15, #0x3, r5, c5, c2, #0x3
     2b4: 5f5f0064     	svcpl	#0x5f0064
     2b8: 5f756d6d     	svcpl	#0x756d6d
     2bc: 5f6c6274     	svcpl	#0x6c6274
     2c0: 72617473     	rsbvc	r7, r1, #1929379840
     2c4: 5f5f0074     	svcpl	#0x5f0074
     2c8: 5f756d6d     	svcpl	#0x756d6d
     2cc: 5f6c6274     	svcpl	#0x6c6274
     2d0: 00646e65     	rsbeq	r6, r4, r5, ror #28
     2d4: 78655f5f     	stmdavc	r5!, {r0, r1, r2, r3, r4, r6, r8, r9, r10, r11, r12, lr} ^
     2d8: 5f786469     	svcpl	#0x786469
     2dc: 72617473     	rsbvc	r7, r1, #1929379840
     2e0: 5f5f0074     	svcpl	#0x5f0074
     2e4: 64697865     	strbtvs	r7, [r9], #-2149
     2e8: 6e655f78     	mcrvs	p15, #0x3, r5, c5, c8, #0x3
     2ec: 5f5f0064     	svcpl	#0x5f0064
     2f0: 69657270     	stmdbvs	r5!, {r4, r5, r6, r9, r12, sp, lr} ^
     2f4: 5f74696e     	svcpl	#0x74696e
     2f8: 61727261     	cmnvs	r2, r1, ror #4
     2fc: 74735f79     	ldrbtvc	r5, [r3], #-3961
     300: 00747261     	rsbseq	r7, r4, r1, ror #4
     304: 72705f5f     	rsbsvc	r5, r0, #380
     308: 696e6965     	stmdbvs	lr!, {r0, r2, r5, r6, r8, r11, sp, lr} ^
     30c: 72615f74     	rsbvc	r5, r1, #116, #30
     310: 5f796172     	svcpl	#0x796172
     314: 00646e65     	rsbeq	r6, r4, r5, ror #28
     318: 6e695f5f     	mcrvs	p15, #0x3, r5, c9, c15, #0x2
     31c: 615f7469     	cmpvs	pc, r9, ror #8
     320: 79617272     	stmdbvc	r1!, {r1, r4, r5, r6, r9, r12, sp, lr} ^
     324: 6174735f     	cmnvs	r4, pc, asr r3
     328: 5f007472     	svcpl	#0x7472
     32c: 696e695f     	stmdbvs	lr!, {r0, r1, r2, r3, r4, r6, r8, r11, sp, lr} ^
     330: 72615f74     	rsbvc	r5, r1, #116, #30
     334: 5f796172     	svcpl	#0x796172
     338: 00646e65     	rsbeq	r6, r4, r5, ror #28
     33c: 69665f5f     	stmdbvs	r6!, {r0, r1, r2, r3, r4, r6, r8, r9, r10, r11, r12, lr} ^
     340: 615f696e     	cmpvs	pc, lr, ror #18
     344: 79617272     	stmdbvc	r1!, {r1, r4, r5, r6, r9, r12, sp, lr} ^
     348: 6174735f     	cmnvs	r4, pc, asr r3
     34c: 5f007472     	svcpl	#0x7472
     350: 6e69665f     	mcrvs	p6, #0x3, r6, c9, c15, #0x2
     354: 72615f69     	rsbvc	r5, r1, #420
     358: 5f796172     	svcpl	#0x796172
     35c: 00646e65     	rsbeq	r6, r4, r5, ror #28
     360: 52415f5f     	subpl	r5, r1, #380
     364: 74612e4d     	strbtvc	r2, [r1], #-3661
     368: 62697274     	rsbvs	r7, r9, #116, #4
     36c: 73657475     	<unknown>
     370: 6174735f     	cmnvs	r4, pc, asr r3
     374: 5f007472     	svcpl	#0x7472
     378: 4d52415f     	ldclmi	p1, c4, [r2, #-380]
     37c: 7474612e     	ldrbtvc	r6, [r4], #-302
     380: 75626972     	strbvc	r6, [r2, #-0x972]!
     384: 5f736574     	svcpl	#0x736574
     388: 00646e65     	rsbeq	r6, r4, r5, ror #28
     38c: 64735f5f     	ldrbtvs	r5, [r3], #-3935
     390: 5f617461     	svcpl	#0x617461
     394: 00646e65     	rsbeq	r6, r4, r5, ror #28
     398: 64745f5f     	ldrbtvs	r5, [r4], #-3935
     39c: 5f617461     	svcpl	#0x617461
     3a0: 72617473     	rsbvc	r7, r1, #1929379840
     3a4: 5f5f0074     	svcpl	#0x5f0074
     3a8: 74616474     	strbtvc	r6, [r1], #-1140
     3ac: 6e655f61     	cdpvs	p15, #0x6, c5, c5, c1, #0x3
     3b0: 5f5f0064     	svcpl	#0x5f0064
     3b4: 73736274     	cmnvc	r3, #116, #4
     3b8: 6174735f     	cmnvs	r4, pc, asr r3
     3bc: 5f007472     	svcpl	#0x7472
     3c0: 7362745f     	<unknown>
     3c4: 6e655f73     	mcrvs	p15, #0x3, r5, c5, c3, #0x3
     3c8: 535f0064     	cmppl	pc, #100
     3cc: 425f4144     	subsmi	r4, pc, #68, #2
     3d0: 5f455341     	svcpl	#0x455341
     3d4: 44535f00     	ldrbmi	r5, [r3], #-3840
     3d8: 425f3241     	subsmi	r3, pc, #268435460
     3dc: 5f455341     	svcpl	#0x455341
     3e0: 65685f00     	strbvs	r5, [r8, #-0xf00]!
     3e4: 48007061     	stmdami	r0, {r0, r5, r6, r12, sp, lr}
     3e8: 42706165     	rsbsmi	r6, r0, #1073741849
     3ec: 00657361     	rsbeq	r7, r5, r1, ror #6
     3f0: 6165685f     	qdsubvs	r6, pc, r5
     3f4: 74735f70     	ldrbtvc	r5, [r3], #-3952
     3f8: 00747261     	rsbseq	r7, r4, r1, ror #4
     3fc: 6165685f     	qdsubvs	r6, pc, r5
     400: 6e655f70     	mcrvs	p15, #0x3, r5, c5, c0, #0x3
     404: 65480064     	strbvs	r0, [r8, #-0x64]
     408: 694c7061     	stmdbvs	r12, {r0, r5, r6, r12, sp, lr} ^
     40c: 0074696d     	rsbseq	r6, r4, sp, ror #18
     410: 6174735f     	cmnvs	r4, pc, asr r3
     414: 655f6b63     	ldrbvs	r6, [pc, #-0xb63]       @ 0xfffffffffffff8b9 <_end+0xfffffffffc6f80a9>
     418: 5f00646e     	svcpl	#0x646e
     41c: 6174735f     	cmnvs	r4, pc, asr r3
     420: 5f006b63     	svcpl	#0x6b63
     424: 5f717269     	svcpl	#0x717269
     428: 63617473     	<unknown>
     42c: 6e655f6b     	cdpvs	p15, #0x6, c5, c5, c11, #0x3
     430: 5f5f0064     	svcpl	#0x5f0064
     434: 5f717269     	svcpl	#0x717269
     438: 63617473     	<unknown>
     43c: 735f006b     	cmpvc	pc, #107
     440: 72657075     	rsbvc	r7, r5, #117
     444: 6f736976     	svcvs	#0x736976
     448: 74735f72     	ldrbtvc	r5, [r3], #-3954
     44c: 5f6b6361     	svcpl	#0x6b6361
     450: 00646e65     	rsbeq	r6, r4, r5, ror #28
     454: 75735f5f     	ldrbvc	r5, [r3, #-0xf5f]!
     458: 76726570     	uhsub16vc	r6, r2, r0
     45c: 726f7369     	rsbvc	r7, pc, #-1543503871
     460: 6174735f     	cmnvs	r4, pc, asr r3
     464: 5f006b63     	svcpl	#0x6b63
     468: 726f6261     	rsbvc	r6, pc, #268435462
     46c: 74735f74     	ldrbtvc	r5, [r3], #-3956
     470: 5f6b6361     	svcpl	#0x6b6361
     474: 00646e65     	rsbeq	r6, r4, r5, ror #28
     478: 62615f5f     	rsbvs	r5, r1, #380
     47c: 5f74726f     	svcpl	#0x74726f
     480: 63617473     	<unknown>
     484: 665f006b     	ldrbvs	r0, [pc], -r11, rrx
     488: 735f7169     	cmpvc	pc, #1073741850
     48c: 6b636174     	blvs	0x18d8a64 <.symtab+0x18d8a64> @ imm = #0x18d85d0
     490: 646e655f     	strbtvs	r6, [lr], #-1375
     494: 665f5f00     	ldrbvs	r5, [pc], -r0, lsl #30
     498: 735f7169     	cmpvc	pc, #1073741850
     49c: 6b636174     	blvs	0x18d8a74 <.symtab+0x18d8a74> @ imm = #0x18d85d0
     4a0: 6e755f00     	cdpvs	p15, #0x7, c5, c5, c0, #0x0
     4a4: 5f666564     	svcpl	#0x666564
     4a8: 63617473     	<unknown>
     4ac: 6e655f6b     	cdpvs	p15, #0x6, c5, c5, c11, #0x3
     4b0: 5f5f0064     	svcpl	#0x5f0064
     4b4: 65646e75     	strbvs	r6, [r4, #-0xe75]!
     4b8: 74735f66     	ldrbtvc	r5, [r3], #-3942
     4bc: 006b6361     	rsbeq	r6, r11, r1, ror #6
     4c0: 646e655f     	strbtvs	r6, [lr], #-1375
     4c4: 00           	<unknown>
