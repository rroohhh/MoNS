	;; rXb:  8bit
	;; rXw: 16bit
	;; rXd: 31bit
	;; rX : 64bit
	;; rsp: Stack pointer
	;; rbp: Stack frame base pointer
	;; rdi, rsi, rdx, rcx, r8d, r9d: function arguments
	;; rdi: address of the gravity_naive struct
	;; layout of gravity_naive struct:
	;; 0x0  u64        size
	;; 0x8  double *   mass
	;; 0x10 v3     *   positions
	;; 0x18 v3     *   accelerations

	;; syntax: instr source dest

	;; function call header
	push   %rbp                                                               0x44c2c0 <>
	push   %r15	                            								  0x44c2c1 <+1>
	push   %r14                             								  0x44c2c3 <+3>
	push   %r13                             								  0x44c2c5 <+5>
	push   %r12                             								  0x44c2c7 <+7>
	push   %rbx                             								  0x44c2c9 <+9>
	;; load size to %r12
	mov    (%rdi),%r12  													  0x44c2ca <+10>
	;; test if size <= 0
	test   %r12,%r12														  0x44c2cd <+13>
	;; if size <= jump to return
	jle    0x44c7cc <mons::gravity_naive::apply(double)+1292>				  0x44c2d0 <+16>
	;; load accelerations pointer to %r15
	mov    0x18(%rdi),%r15													  0x44c2d6 <+22>
	;; decrement %12 (bodycount) by one
	lea    -0x1(%r12),%r14													  0x44c2da <+26>
	;; %rax = %r12 + 2 * %r12 = 3 * bodycount
	lea    (%r12,%r12,2),%rax												  0x44c2df <+31>
	;; copy %rax = 3 * bodycount to stack
	mov    %rax,-0x30(%rsp)													  0x44c2e3 <+35>
	;; %rax = &accelerations[bodycount]
	lea    (%r15,%rax,8),%rax												  0x44c2e8 <+40>
	;; copy %rax = &accelerations[bodycount] to stack
	mov    %rax,-0x38(%rsp)													  0x44c2ec <+44>
	;; %r10 = accelerations + 0x70 ??? = acceleration[4].pos[2]
	lea    0x70(%r15),%r10													  0x44c2f1 <+49>
	;; %rax = &accelerations[0].pos[2]
	lea    0x10(%r15),%rax													  0x44c2f5 <+53>
	;; copy %rax = &accelerations[0].pos[2] to stack
	mov    %rax,-0x18(%rsp)													  0x44c2f9 <+57>
	;; %eax = 0
	xor    %eax,%eax														  0x44c2fe <+62>
	;; %ecx = 1
	mov    $0x1,%ecx														  0x44c300 <+64>
	;; %r11d = 4
	mov    $0x4,%r11d														  0x44c305 <+69>
	;; wtf???, %xmm0 = {1.0, 0}
	vmovsd 0x4b205(%rip),%xmm0        # 0x497518							  0x44c30b <+75>
	;; %ymm12 = 0
	vxorpd %ymm12,%ymm12,%ymm12												  0x44c313 <+83>
	;; store bodycount - 1 in Stack
	mov    %r14,-0x10(%rsp)													  0x44c318 <+88>
	;; store struct pointer in stack
	mov    %rdi,-0x68(%rsp)													  0x44c31d <+93>
	;; store bodycount in Stack
	mov    %r12,-0x70(%rsp)													  0x44c322 <+98>
	;; store accelerations pointer in Stack
	mov    %r15,-0x50(%rsp)													  0x44c327 <+103>
	;; jump
	jmp    0x44c360 <mons::gravity_naive::apply(double)+160>				  0x44c32c <+108>
	;; %rsi = %rdi
	mov    %rdi,%rsi														  0x44c32e <+110>
	;; %rdi = struct pointer
	mov    -0x68(%rsp),%rdi													  0x44c331 <+113>
	;; %r12 = bodycount
	mov    -0x70(%rsp),%r12													  0x44c336 <+118>
	jmpq   0x44c6b8 <mons::gravity_naive::apply(double)+1016>				  0x44c33b <+123>
	;; %rsi = old %rax + 1
	mov    -0x60(%rsp),%rsi													  0x44c340 <+128>
	;; %rdi = struct pointer
	mov    -0x68(%rsp),%rdi													  0x44c345 <+133>
	;; %r12 = bodycount
	mov    -0x70(%rsp),%r12													  0x44c34a <+138>
	;; %r15 = accelerations pointer
	mov    -0x50(%rsp),%r15													  0x44c34f <+143>
	jmpq   0x44c6b3 <mons::gravity_naive::apply(double)+1011>				  0x44c354 <+148>
	;; %rsi = old %rax + 1
	mov    -0x60(%rsp),%rsi													  0x44c359 <+153>
	jmp    0x44c331 <mons::gravity_naive::apply(double)+113>				  0x44c35e <+158>
	;; %rbx = %rax + 1
	lea    0x1(%rax),%rbx													  0x44c360 <+160>
	;; %rdx = 3 * %rax
	lea    (%rax,%rax,2),%rdx												  0x44c364 <+164>
	;; %rsi = &accelerations[%rax]
	lea    (%r15,%rdx,8),%rsi												  0x44c368 <+168>
	;; %xmm3 = {accelerations[%rax].pos[0], accelerations[%rax].pox[1]}
	vmovupd (%r15,%rdx,8),%xmm3												  0x44c36c <+172>
	;; %xmm2 = {accelerations[%rax].pos[2], 0}
	vmovsd 0x10(%r15,%rdx,8),%xmm2											  0x44c372 <+178>
	;; jump if %rbx >= %r12 = bodycount
	cmp    %r12,%rbx														  0x44c379 <+185>
	jge    0x44c6a0 <mons::gravity_naive::apply(double)+992>				  0x44c37c <+188>
	;; store %rcx to stack
	mov    %rcx,-0x58(%rsp)													  0x44c382 <+194>
	;; get bodycount -1 from stack
	mov    -0x10(%rsp),%r9													  0x44c387 <+199>
	;; %r9 = bodycount - 1 - %rax
	sub    %rax,%r9															  0x44c38c <+204>
	;; %rcx = masses
	mov    0x8(%rdi),%rcx													  0x44c38f <+207>
	;; %rbp = positions
	mov    0x10(%rdi),%rbp													  0x44c393 <+211>
	;; %r13 = &positions[%rax]
	lea    0x0(%rbp,%rdx,8),%r13											  0x44c397 <+215>
	;; store positions pointer to stack
	mov    %rbp,-0x40(%rsp)													  0x44c39c <+220>
	;; %rbp = &positions[%rax].pos[2]
	lea    0x10(%rbp,%rdx,8),%rbp											  0x44c3a1 <+225>
	;; store masses pointer to stack
	mov    %rcx,-0x48(%rsp)													  0x44c3a6 <+230>
	;; %r8 = &masses[%rax]
	lea    (%rcx,%rax,8),%r8												  0x44c3ab <+235>
	;; compare 3 and bodycount - 1 - %rax
	cmp    $0x3,%r9															  0x44c3af <+239>
	;; store %rbx = %rax + 1 in stack
	mov    %rbx,-0x60(%rsp)													  0x44c3b3 <+243>
	;; store &acceleration[%rax] in stack
	mov    %rsi,-0x8(%rsp)													  0x44c3b8 <+248>
	;; jump if bodycount - 1 - %rax <= 3
	jbe    0x44c6b0 <mons::gravity_naive::apply(double)+1008>				  0x44c3bd <+253>
	;; %rcx = bodycount - 1 - %rax
	mov    %r9,%rcx															  0x44c3c3 <+259>
	;; %rcx = (bodycount - 1 - %rax) - (bodycount - 1 - %rax) % 4, fancy trick for that
	and    $0xfffffffffffffffc,%rcx											  0x44c3c6 <+262>
	;; store %rcx to stack
	mov    %rcx,-0x20(%rsp)													  0x44c3ca <+266>
	;; wtf????
	je     0x44c6b0 <mons::gravity_naive::apply(double)+1008>				  0x44c3cf <+271>
	;; store bodycount - 1 - %rax to stack
	mov    %r9,-0x28(%rsp)													  0x44c3d5 <+277>
	;; %rsi = &accelerations[%rax + 1]
	lea    0x18(%r15,%rdx,8),%rsi											  0x44c3da <+282>
	;; %rax = 3 * %rbx = 3 * (%rax + 1)
	lea    (%rbx,%rbx,2),%rax												  0x44c3df <+287>
	;; %r15 = positions
	mov    -0x40(%rsp),%r15													  0x44c3e3 <+291>
	;; %rax = &positions[%rax / 3] = &position[%rbx]
	lea    (%r15,%rax,8),%rax												  0x44c3e8 <+296>
	;; %rdi = 3 * bodycount
	mov    -0x30(%rsp),%rdi													  0x44c3ec <+300>
	;; %rbx = &positions[%rdi / 3] = &positions[bodycount]
	lea    (%r15,%rdi,8),%rbx												  0x44c3f1 <+305>
	;; %rcx = masses
	mov    -0x48(%rsp),%rcx													  0x44c3f5 <+309>
	;; %rdi = &masses[bodycount]
	lea    (%rcx,%r12,8),%rdi												  0x44c3fa <+314>
	;; CF =  %rsi < %rbx = 1 : 0
	;; CF =  &accelerations[old %rax + 1] < %positions[bodycount] = 1 : 0
	cmp    %rbx,%rsi														  0x44c3fe <+318>
	;; %bl = -CF
	sbb    %bl,%bl															  0x44c401 <+321>
	;; %r9 = &accelerations[bodycount]
	mov    -0x38(%rsp),%r9													  0x44c403 <+323>
	;; CF =  %rax < %r9 = 1 : 0
	;; CF =  &positions[old %rax] < &accelerations[bodycount] = 1 : 0
	cmp    %r9,%rax															  0x44c408 <+328>
	;; %r15b = -CF
	sbb    %r15b,%r15b														  0x44c40b <+331>
	;; %r15b &= %bl
	and    %bl,%r15b														  0x44c40e <+334>
	;; CF =  %rsi < %rbp = 1 : 0
	;; CF =  &accelerations[old %rax + 1] < &positions[old %rax].pos[2] = 1 : 0
	cmp    %rbp,%rsi														  0x44c411 <+337>
	;; %bl = -CF
	sbb    %bl,%bl															  0x44c414 <+340>
	;; CF =  %rbp < %r9 = 1 : 0
	;; CF = &positions[old %rax].pos[2] < &accelerations[bodycount] = 1 : 0
	cmp    %r9,%rbp															  0x44c416 <+342>
	;; %al = -CF
	sbb    %al,%al															  0x44c419 <+345>
	;; store %al to stack
	mov    %al,-0x71(%rsp)													  0x44c41b <+347>
	;; CF =  %rsi < %r8 = 1 : 0
	;; CF = &accelerations[old %rax + 1] < &masses[old %rax] = 1 : 0
	cmp    %r8,%rsi															  0x44c41f <+351>
	;; %al = -CF
	sbb    %al,%al															  0x44c422 <+354>
	;; CF =  %r8 < %r9 = 1 : 0
	;; CF = &masses[old %rax] < &accelerations[bodycount] = 1 : 0
	cmp    %r9,%r8															  0x44c424 <+356>
	;; %r12b = -CF
	sbb    %r12b,%r12b														  0x44c427 <+359>
	;; CF =  %rsi < %rdi = 1 : 0
	;; CF = &accelerations[old %rax + 1] < &masses[bodycount] = 1 : 0
	cmp    %rdi,%rsi														  0x44c42a <+362>
	;; %rsi = old %rax + 1
	mov    -0x60(%rsp),%rsi													  0x44c42d <+365>
	;; %rdi = &masses[old %rax + 1]
	lea    (%rcx,%rsi,8),%rdi												  0x44c432 <+370>
	;; %sil = -CF
	sbb    %sil,%sil														  0x44c436 <+374>
	;; CF =  %rdi < %r9 = 1 : 0
	;; CF = %rdi = &masses[old %rax + 1] < &accelerations[bodycount] = 1 : 0
	cmp    %r9,%rdi															  0x44c439 <+377>
	;; %r9b = -CF
	sbb    %r9b,%r9b														  0x44c43c <+380>
	;; ZF = %r15b == 1 ? 0 : 1
	test   $0x1,%r15b														  0x44c43f <+383>
	;; jump if %r15b == 0x1
	jne    0x44c340 <mons::gravity_naive::apply(double)+128>				  0x44c443 <+387>
	;; %bl &= stack %al
	and    -0x71(%rsp),%bl													  0x44c449 <+393>
	;; %bl &= 1
	and    $0x1,%bl															  0x44c44d <+397>
	;; r15 = acceleration
	mov    -0x50(%rsp),%r15													  0x44c450 <+400>
	;; %rcx = some value frome the stack (in the beginning 0x1)
	mov    -0x58(%rsp),%rcx													  0x44c455 <+405>
	;; jump if %bl == 0x1
	jne    0x44c359 <mons::gravity_naive::apply(double)+153>				  0x44c45a <+410>
	;; %al &= %r12b
	and    %r12b,%al														  0x44c460 <+416>
	;; %al &= 1
	and    $0x1,%al															  0x44c463 <+419>
	;; %rdi = old %rax + 1
	mov    -0x60(%rsp),%rdi													  0x44c465 <+421>
	;; jump if %al == 0x1
	jne    0x44c32e <mons::gravity_naive::apply(double)+110>				  0x44c46a <+426>
	;; %sil &= r9b
	and    %r9b,%sil														  0x44c470 <+432>
	;; %sil &= 1
	and    $0x1,%sil														  0x44c473 <+435>
	;; jump if %sil == 0x1
	jne    0x44c32e <mons::gravity_naive::apply(double)+110>				  0x44c477 <+439>
	;; %rax = bodycount - 1
	mov    %r14,%rax														  0x44c47d <+445>
	;; %rax = (bodycount - 1) - (bodycount - 1) % 4
	and    $0xfffffffffffffffc,%rax											  0x44c480 <+448>
	;; %15 = positions
	mov    -0x40(%rsp),%r15													  0x44c484 <+452>
	;; %rdx = positions[old %rax].pos[1]
	lea    0x8(%r15,%rdx,8),%rdx											  0x44c489 <+457>
	;; %rbx = (bodycount - 1) - (bodycount - 1) % 4
	mov    -0x20(%rsp),%rbx													  0x44c48e <+462>
	;; %rsi = %rdi + %rbx = %rbx + old %rax + 1
	lea    (%rdi,%rbx,1),%rsi												  0x44c493 <+467>
	;; %rdi = %rbx
	mov    %rbx,%rdi														  0x44c497 <+471>


	;; old %rax == i
	;; outer really loop starts here

	;; wtf?????
	;; %ymm3  = {a, b, c, d} = {accelerations[old %rax].pos[0], accelerations[old %rax].pos[1], 0, 0}
	;; %ymm12 = {e, f, g, h} = {0, 0, 0, 0}
	;; %ymm4  = {i, j, k, l} = {?, ?, ?, ?}
	;; %ymm4 = {a, 0, 0, 0} = {accelerations[old %rax].pos[0], 0, 0, 0}
	vblendpd $0x1,%ymm3,%ymm12,%ymm4										  0x44c49a <+474>
	;; %xmm3 = {a, b} -> {b, a}
	vpermilpd $0x1,%xmm3,%xmm3												  0x44c4a0 <+480>
	;; %ymm3 {b, a, c, d} -> {b, 0, 0, 0} = {accelerations[old %rax].pos[1], 0, 0, 0}
	vblendpd $0x1,%ymm3,%ymm12,%ymm3										  0x44c4a6 <+486>
	;; %ymm2 {a, b, c, d} -> {a, 0, 0, 0} = {accelerations[old %rax].pos[2], 0, 0, 0}
	vblendpd $0x1,%ymm2,%ymm12,%ymm2										  0x44c4ac <+492>
	;; %ymm13 = {positions[old %rax].pos[0], positions[old %rax].pos[0], positions[old %rax].pos[0], positions[old %rax].pos[0]}
	vbroadcastsd 0x0(%r13),%ymm13											  0x44c4b2 <+498>
	;; %ymm14 = {positions[old %rax].pos[1], positions[old %rax].pos[1], positions[old %rax].pos[1], positions[old %rax].pos[1]}
	vbroadcastsd (%rdx),%ymm14												  0x44c4b8 <+504>
	;; %ymm15 = {positions[old %rax].pos[2], positions[old %rax].pos[2], positions[old %rax].pos[2], positions[old %rax].pos[2]}
	vbroadcastsd 0x0(%rbp),%ymm15											  0x44c4bd <+509>
	;; %ymm8 = {masses[old %rax], masses[old %rax], masses[old %rax], masses[old %rax]}
	vbroadcastsd (%r8),%ymm8												  0x44c4c3 <+515>
	;; %rdx = masses
	mov    -0x48(%rsp),%rdx													  0x44c4c8 <+520>
	;; %r9 = &masses[%rcx]
	lea    (%rdx,%rcx,8),%r9												  0x44c4cd <+525>
	;; %rdx = 3 * %r11
	lea    (%r11,%r11,2),%rdx												  0x44c4d1 <+529>
	;; %r15 = &positions[%r11].pos[2] = &positions[j + 3].pos[2]
	lea    0x10(%r15,%rdx,8),%r15											  0x44c4d5 <+533>
	;; %rdx = %r10 = &accelerations[j + 3].pos[2]
	mov    %r10,%rdx														  0x44c4da <+538>
	;; nop
	nopl   (%rax)															  0x44c4dd <+541>

	;; inner loop
	;; %rcx = j
	;; %r15 = &positions[j + 3].pos[2]
	;; %r9 = &masses[j]
	;; %rdx = &accelerations[j + 3].pos[2]
	;; %xmm1 = {positions[j + 2].pos[0], 0}
	vmovsd -0x28(%r15),%xmm1												  0x44c4e0 <+544>
	;; %xmm1 = {positions[j + 2].pos[0], positions[j + 3].pos[0]}
	vmovhpd -0x10(%r15),%xmm1,%xmm1											  0x44c4e6 <+550>
	;; %xmm5 = {positions[j].pos[0], 0}
	vmovsd -0x58(%r15),%xmm5												  0x44c4ec <+556>
	;; %xmm6 = {positions[j].pos[1], 0}
	vmovsd -0x50(%r15),%xmm6												  0x44c4f2 <+562>
	;; %xmm5 = {positions[j].pos[0], positions[j + 1]}
	vmovhpd -0x40(%r15),%xmm5,%xmm5											  0x44c4f8 <+568>
	;; %ymm1 = {positions[j].pos[0], positions[j + 1].pos[0], positions[j + 2].pos[0], positions[j + 3].pos[0]}
	vinsertf128 $0x1,%xmm1,%ymm5,%ymm1										  0x44c4fe <+574>
	;; %xmm5 = {positions[j + 2].pos[1], ~}
	vmovsd -0x20(%r15),%xmm5												  0x44c504 <+580>
	;; %xmm5 = {positions[j + 2].pos[1], positions[j + 3].pos[1]}
	vmovhpd -0x8(%r15),%xmm5,%xmm5											  0x44c50a <+586>
	;; %ymm1 = %ymm13 - %ymm1 == dx
	vsubpd %ymm1,%ymm13,%ymm1												  0x44c510 <+592>
	;; %xmm6 = {positions[j].pos[1], positions[j + 1].pos[1]}
	vmovhpd -0x38(%r15),%xmm6,%xmm6											  0x44c514 <+596>
	;; %xmm7 = {positions[j + 2].pos[2], 0}
	vmovsd -0x18(%r15),%xmm7												  0x44c51a <+602>
	;; %xmm7 = {positions[j + 2].pos[2], positions[j + 3].pos[2]}
	vmovhpd (%r15),%xmm7,%xmm7												  0x44c520 <+608>
	;; %ymm5 = {positions[j].pos[1], positions[j + 1].pos[1], positions[j + 2].pos[1], positions[j + 3].pos[1]}
	vinsertf128 $0x1,%xmm5,%ymm6,%ymm5										  0x44c525 <+613>
	;; %xmm7 = {positions[j + 0].pos[2], ~}
	vmovsd -0x48(%r15),%xmm6												  0x44c52b <+619>
	;; %xmm7 = {positions[j + 0].pos[2], positions[j + 1].pos[2]}
	vmovhpd -0x30(%r15),%xmm6,%xmm6											  0x44c531 <+625>
	;; %ymm6 = {positions[j].pos[2], positions[j + 1].pos[2], positions[j + 2].pos[2], positions[j + 3].pos[2]}
	vinsertf128 $0x1,%xmm7,%ymm6,%ymm6										  0x44c537 <+631>
	;; %ymm10 = %ymm14 - %ymm5 = dy
	vsubpd %ymm5,%ymm14,%ymm10												  0x44c53d <+637>
	;; %ymm9 = %ymm15 - %ymm6 = dz
	vsubpd %ymm6,%ymm15,%ymm9												  0x44c541 <+641>
	;; %ymm5 = %ymm1 * %ymm1 = dx * dx
	vmulpd %ymm1,%ymm1,%ymm5												  0x44c545 <+645>
	;; %ymm5 += %ymm10 * %ymm10 = dx * dx + dy * dy
	vfmadd231pd %ymm10,%ymm10,%ymm5											  0x44c549 <+649>
	;; %ymm5 += %ymm9 * %ymm9 = dx * dx + dy * dy + dz * dz
	vfmadd231pd %ymm9,%ymm9,%ymm5											  0x44c54e <+654>
	;; %ymm5 = sqrt(%ymm5)
	vsqrtpd %ymm5,%ymm5														  0x44c553 <+659>
	;; %ymm6 = {1.0, 1.0, 1.0, 1.0}
	vbroadcastsd 0x4afb8(%rip),%ymm6        # 0x497518						  0x44c557 <+663>
	;; %ymm5 = 1 / %ymm5 = 1 / sqrt(dist)
	vdivpd %ymm5,%ymm6,%ymm5												  0x44c560 <+672>
	;; %ymm6 = %ymm5 * %ymm5
	vmulpd %ymm5,%ymm5,%ymm6												  0x44c564 <+676>
	;; %ymm11 = %ymm6 * %ymm5 = 1 / sqrt(dist)^3
	vmulpd %ymm5,%ymm6,%ymm11												  0x44c568 <+680>
	;; %xmm5 = {accelerations[j].pos[0], 0}
	vmovsd -0x58(%rdx),%xmm5												  0x44c56c <+684>
	;; %xmm6 = {accelerations[j + 2].pos[0], 0}
	vmovsd -0x28(%rdx),%xmm6												  0x44c571 <+689>
	;; %ymm1 = dx / sqrt(dist)^3
	vmulpd %ymm1,%ymm11,%ymm1												  0x44c576 <+694>
	;; %xmm6 = {accelerations[j + 2].pos[0], accelerations[j + 3].pos[0]}
	vmovhpd -0x10(%rdx),%xmm6,%xmm6											  0x44c57a <+698>
	;; %xmm5 = {accelerations[j].pos[0], accelerations[j + 1].pos[0]}
	vmovhpd -0x40(%rdx),%xmm5,%xmm5											  0x44c57f <+703>
	;; %ymm4 += %r9 * %ymm1 = masses[j] * dx / sqrt(dist)^3
	vfnmadd231pd (%r9),%ymm1,%ymm4											  0x44c584 <+708>
	;; %ymm5 = {accelerations[j].pos[0], accelerations[j + 1].pos[0], accelerations[j + 2].pos[0], accelerations[j + 3].pos[0]}
	vinsertf128 $0x1,%xmm6,%ymm5,%ymm5										  0x44c589 <+713>
	;; %ymm1 = %ymm5 + %ymm1 * %ymm8 = masses[i] * dx / sqrt(dist)^3
	vfmadd213pd %ymm5,%ymm8,%ymm1											  0x44c58f <+719>
	;; accelerations[j].pos[0] = %xmm1[0]
	vmovlpd %xmm1,-0x58(%rdx)												  0x44c594 <+724>
	;; accelerations[j + 1].pos[0] = %xmm1[1]
	vmovhpd %xmm1,-0x40(%rdx)												  0x44c599 <+729>
	vextractf128 $0x1,%ymm1,%xmm1											  0x44c59e <+734>
	vmovlpd %xmm1,-0x28(%rdx)												  0x44c5a4 <+740>
	vmovhpd %xmm1,-0x10(%rdx)												  0x44c5a9 <+745>
	vmovsd -0x50(%rdx),%xmm1												  0x44c5ae <+750>
	vmovsd -0x20(%rdx),%xmm5												  0x44c5b3 <+755>
	vmovhpd -0x8(%rdx),%xmm5,%xmm5											  0x44c5b8 <+760>
	vmovhpd -0x38(%rdx),%xmm1,%xmm1											  0x44c5bd <+765>
	vmulpd %ymm10,%ymm11,%ymm6												  0x44c5c2 <+770>
	vinsertf128 $0x1,%xmm5,%ymm1,%ymm1										  0x44c5c7 <+775>
	vfmadd231pd %ymm6,%ymm8,%ymm1											  0x44c5cd <+781>
	vfnmadd231pd (%r9),%ymm6,%ymm3											  0x44c5d2 <+786>
	vmovlpd %xmm1,-0x50(%rdx)												  0x44c5d7 <+791>
	vmovhpd %xmm1,-0x38(%rdx)												  0x44c5dc <+796>
	vextractf128 $0x1,%ymm1,%xmm1											  0x44c5e1 <+801>
	vmovlpd %xmm1,-0x20(%rdx)												  0x44c5e7 <+807>
	vmovhpd %xmm1,-0x8(%rdx)												  0x44c5ec <+812>
	vmulpd %ymm9,%ymm11,%ymm1												  0x44c5f1 <+817>
	vfnmadd231pd (%r9),%ymm1,%ymm2											  0x44c5f6 <+822>
	vmovsd -0x48(%rdx),%xmm5												  0x44c5fb <+827>
	vmovsd -0x18(%rdx),%xmm6												  0x44c600 <+832>
	vmovhpd (%rdx),%xmm6,%xmm6												  0x44c605 <+837>
	vmovhpd -0x30(%rdx),%xmm5,%xmm5											  0x44c609 <+841>
	vinsertf128 $0x1,%xmm6,%ymm5,%ymm5										  0x44c60e <+846>
	vfmadd231pd %ymm1,%ymm8,%ymm5											  0x44c614 <+852>
	vmovlpd %xmm5,-0x48(%rdx)												  0x44c619 <+857>
	vmovhpd %xmm5,-0x30(%rdx)												  0x44c61e <+862>
	vextractf128 $0x1,%ymm5,%xmm1											  0x44c623 <+867>
	vmovlpd %xmm1,-0x18(%rdx)												  0x44c629 <+873>
	vmovhpd %xmm1,(%rdx)													  0x44c62e <+878>
	add    $0x60,%rdx														  0x44c632 <+882>
	add    $0x20,%r9														  0x44c636 <+886>
	add    $0x60,%r15														  0x44c63a <+890>
	add    $0xfffffffffffffffc,%rax											  0x44c63e <+894>
	jne    0x44c4e0 <mons::gravity_naive::apply(double)+544>				  0x44c642 <+898>
	vextractf128 $0x1,%ymm2,%xmm1											  0x44c648 <+904>
	vaddpd %ymm1,%ymm2,%ymm1												  0x44c64e <+910>
	vhaddpd %ymm1,%ymm1,%ymm2												  0x44c652 <+914>
	vextractf128 $0x1,%ymm3,%xmm1											  0x44c656 <+918>
	vaddpd %ymm1,%ymm3,%ymm1												  0x44c65c <+924>
	vhaddpd %ymm1,%ymm1,%ymm1												  0x44c660 <+928>
	vextractf128 $0x1,%ymm4,%xmm3											  0x44c664 <+932>
	vaddpd %ymm3,%ymm4,%ymm3												  0x44c66a <+938>
	vhaddpd %ymm3,%ymm3,%ymm3												  0x44c66e <+942>
	vunpcklpd %xmm1,%xmm3,%xmm3												  0x44c672 <+946>
	cmp    %rdi,-0x28(%rsp)													  0x44c676 <+950>
	mov    -0x68(%rsp),%rdi													  0x44c67b <+955>
	mov    -0x70(%rsp),%r12													  0x44c680 <+960>
	mov    -0x50(%rsp),%r15													  0x44c685 <+965>
	jne    0x44c6b8 <mons::gravity_naive::apply(double)+1016>				  0x44c68a <+970>
	jmpq   0x44c784 <mons::gravity_naive::apply(double)+1220>				  0x44c68c <+972>
	data16 data16 data16 data16 data16 nopw %cs:0x0(%rax,%rax,1)			  0x44c691 <+977>
	vmovapd %xmm2,%xmm4														  0x44c6a0 <+992>
	vmovapd %xmm3,%xmm5														  0x44c6a4 <+996>
	jmpq   0x44c79f <mons::gravity_naive::apply(double)+1247>				  0x44c6a8 <+1000>
	nopl   (%rax)															  0x44c6ad <+1005>
	mov    %rbx,%rsi														  0x44c6b0 <+1008>
	mov    -0x58(%rsp),%rcx													  0x44c6b3 <+1011>
	mov    -0x48(%rsp),%rax													  0x44c6b8 <+1016>
	lea    (%rax,%rsi,8),%rax												  0x44c6bd <+1021>
	mov    %r12,%rdx														  0x44c6c1 <+1025>
	sub    %rsi,%rdx														  0x44c6c4 <+1028>
	shl    $0x3,%rsi														  0x44c6c7 <+1031>
	lea    (%rsi,%rsi,2),%rbx												  0x44c6cb <+1035>
	mov    -0x40(%rsp),%rsi													  0x44c6cf <+1039>
	lea    0x10(%rsi,%rbx,1),%rsi											  0x44c6d4 <+1044>
	add    -0x18(%rsp),%rbx													  0x44c6d9 <+1049>
	xchg   %ax,%ax															  0x44c6de <+1054>
	vmovupd 0x0(%r13),%xmm1													  0x44c6e0 <+1056>
	vsubpd -0x10(%rsi),%xmm1,%xmm1											  0x44c6e6 <+1062>
	vmovsd 0x0(%rbp),%xmm4													  0x44c6eb <+1067>
	vsubsd (%rsi),%xmm4,%xmm4												  0x44c6f0 <+1072>
	vmulsd %xmm1,%xmm1,%xmm5												  0x44c6f4 <+1076>
	vpermilpd $0x1,%xmm1,%xmm6												  0x44c6f8 <+1080>
	vfmadd213sd %xmm5,%xmm6,%xmm6											  0x44c6fe <+1086>
	vfmadd231sd %xmm4,%xmm4,%xmm6											  0x44c703 <+1091>
	vsqrtsd %xmm6,%xmm0,%xmm5												  0x44c708 <+1096>
	vdivsd %xmm5,%xmm0,%xmm5												  0x44c70c <+1100>
	vmulsd %xmm5,%xmm5,%xmm6												  0x44c710 <+1104>
	vmulsd %xmm5,%xmm6,%xmm5												  0x44c714 <+1108>
	vmovsd (%r8),%xmm6														  0x44c718 <+1112>
	vmovddup %xmm5,%xmm7													  0x44c71d <+1117>
	vmulpd %xmm1,%xmm7,%xmm1												  0x44c721 <+1121>
	vmulpd %xmm1,%xmm7,%xmm1												  0x44c721 <+1121>
	vmovapd %xmm1,%xmm7														  0x44c725 <+1125>
	vfmadd213sd -0x10(%rbx),%xmm6,%xmm7										  0x44c729 <+1129>
	vmovsd (%rax),%xmm6														  0x44c72f <+1135>
	vmovsd %xmm7,-0x10(%rbx)												  0x44c733 <+1139>
	vmovhpd (%rax),%xmm6,%xmm6												  0x44c738 <+1144>
	vfnmadd231pd %xmm6,%xmm1,%xmm3											  0x44c73c <+1148>
	vmovsd (%r8),%xmm6														  0x44c741 <+1153>
	vpermilpd $0x1,%xmm1,%xmm1												  0x44c746 <+1158>
	vfmadd213sd -0x8(%rbx),%xmm6,%xmm1										  0x44c74c <+1164>
	vmovsd %xmm1,-0x8(%rbx)													  0x44c752 <+1170>
	vmulsd %xmm4,%xmm5,%xmm1												  0x44c757 <+1175>
	vfnmadd231sd (%rax),%xmm1,%xmm2											  0x44c75b <+1179>
	vmovsd (%r8),%xmm4														  0x44c760 <+1184>
	vfmadd213sd (%rbx),%xmm1,%xmm4											  0x44c765 <+1189>
	vmovsd %xmm4,(%rbx)														  0x44c76a <+1194>
	add    $0x8,%rax														  0x44c76e <+1198>
	add    $0x18,%rsi														  0x44c772 <+1202>
	add    $0x18,%rbx														  0x44c776 <+1206>
	add    $0xffffffffffffffff,%rdx											  0x44c77a <+1210>
	jne    0x44c6e0 <mons::gravity_naive::apply(double)+1056>				  0x44c77e <+1214>
	vmovapd %xmm3,%xmm5														  0x44c784 <+1220>
	vmovapd %xmm2,%xmm4														  0x44c788 <+1224>
	mov    -0x8(%rsp),%rsi													  0x44c78c <+1228>
	vmovupd (%rsi),%xmm3													  0x44c791 <+1233>
	vmovsd 0x10(%rsi),%xmm2													  0x44c795 <+1237>
	mov    -0x60(%rsp),%rbx													  0x44c79a <+1242>
	vsubpd %xmm5,%xmm3,%xmm1												  0x44c79f <+1247>
	vmovupd %xmm1,(%rsi)													  0x44c7a3 <+1251>
	vsubsd %xmm4,%xmm2,%xmm1												  0x44c7a7 <+1255>
	vmovsd %xmm1,0x10(%rsi)													  0x44c7ab <+1259>
	add    $0x18,%r10														  0x44c7b0 <+1264>
	add    $0x1,%rcx														  0x44c7b4 <+1268>
	add    $0x1,%r11														  0x44c7b8 <+1272>
	add    $0xffffffffffffffff,%r14											  0x44c7bc <+1276>
	mov    %rbx,%rax														  0x44c7c0 <+1280>
	mov    %rbx,%rax														  0x44c7c0 <+1280>
	cmp    %r12,%rbx														  0x44c7c3 <+1283>
	jne    0x44c360 <mons::gravity_naive::apply(double)+160>				  0x44c7c6 <+1286>
	pop    %rbx																  0x44c7cc <+1292>
	pop    %r12																  0x44c7cd <+1293>
	pop    %r13																  0x44c7cf <+1295>
	pop    %r14																  0x44c7d1 <+1297>
	pop    %r15																  0x44c7d3 <+1299>
	pop    %rbp																  0x44c7d5 <+1301>
	vzeroupper																  0x44c7d6 <+1302>
	retq																	  0x44c7d9 <+1305>
