import { LavaXCompiler } from '../src/compiler';
import { LavaXAssembler } from '../src/compiler/LavaXAssembler';
import { LavaXVM } from '../src/vm';
import { LocalStorageDriver } from '../src/vm/VFSStorageDriver';

async function test(name: string, source: string, expected?: string) {
  const compiler = new LavaXCompiler();
  const assembler = new LavaXAssembler();
  const vm = new LavaXVM(new LocalStorageDriver());

  const asm = compiler.compile(source);
  if (asm.startsWith('ERROR')) {
    console.log(`[FAIL] ${name}: compile error: ${asm}`);
    return;
  }

  let output = '';
  vm.debug = true; // needed so printf routes through onLog
  vm.onLog = (msg: string) => {
    // filter debug lines, only collect actual program output
    if (!msg.startsWith('[DEBUG]') && !msg.startsWith('System:')) output += msg;
  };

  try {
    vm.load(assembler.assemble(asm));
    await vm.run();
    if (expected !== undefined) {
      const ok = output.includes(expected);
      console.log(`[${ok ? 'PASS' : 'FAIL'}] ${name}: got "${output.trim()}", expected contains "${expected}"`);
    } else {
      console.log(`[INFO] ${name}: output="${output.trim()}"`);
    }
  } catch (e: any) {
    console.log(`[FAIL] ${name}: runtime error: ${e.message}`);
    if (asm && !asm.startsWith('ERROR')) {
      console.log('  ASM:\n' + asm.split('\n').map(l => '    ' + l).join('\n'));
    }
  }
}

async function main() {
  console.log('=== LavaX Pointer Tests ===\n');

  // Test 1: Basic addr pointer, (int *) read
  await test('addr_int_ptr_read', `
void main() {
  int a;
  addr b;
  a = 42;
  b = &a;
  printf("%d\n", (int *)b);
}
`, '42');

  // Test 2: addr with (char *) read
  await test('addr_char_ptr_read', `
void main() {
  char c;
  addr b;
  c = 65;
  b = &c;
  printf("%d\n", (char *)b);
}
`, '65');

  // Test 3: * shorthand for (char *) read
  await test('star_shorthand_char_ptr_read', `
void main() {
  char c;
  addr b;
  c = 72;
  b = &c;
  printf("%d\n", *b);
}
`, '72');

  // Test 4: (long *) dereference
  await test('addr_long_ptr_read', `
void main() {
  long v;
  addr p;
  v = 100000;
  p = &v;
  printf("%ld\n", (long *)p);
}
`, '100000');

  // Test 5: long variable as pointer (addr is long alias)
  await test('long_as_ptr', `
void main() {
  int x;
  long p;
  x = 123;
  p = &x;
  printf("%d\n", (int *)p);
}
`, '123');

  // Test 6: Write through pointer via (int *) = val
  await test('write_through_int_ptr', `
void main() {
  int a;
  addr p;
  a = 0;
  p = &a;
  (int *)p = 99;
  printf("%d\n", a);
}
`, '99');

  // Test 7: Write through (char *) = val
  await test('write_through_char_ptr', `
void main() {
  char c;
  addr p;
  c = 0;
  p = &c;
  (char *)p = 77;
  printf("%d\n", c);
}
`, '77');

  // Test 8: Write through * shorthand = val
  await test('write_through_star', `
void main() {
  char c;
  addr p;
  c = 0;
  p = &c;
  *p = 33;
  printf("%d\n", c);
}
`, '33');

  // Test 9: Pointer passed to function, modify via (int *)
  await test('ptr_func_modify', `
void setval(addr p) {
  (int *)p = 55;
}
void main() {
  int a;
  a = 0;
  setval(&a);
  printf("%d\n", a);
}
`, '55');

  // Test 10: Pointer to specific array element
  await test('ptr_array_element', `
void main() {
  int arr[3];
  addr p;
  arr[0] = 10;
  arr[1] = 20;
  arr[2] = 30;
  p = &arr[1];
  printf("%d\n", (int *)p);
}
`, '20');

  // Test 11: Multiple pointers to different variables
  await test('multiple_pointers', `
void main() {
  int a;
  int b;
  addr pa;
  addr pb;
  a = 10;
  b = 20;
  pa = &a;
  pb = &b;
  printf("%d %d\n", (int *)pa, (int *)pb);
}
`, '10 20');

  // Test 12: Pointer reassignment
  await test('ptr_reassign', `
void main() {
  int a;
  int b;
  addr p;
  a = 11;
  b = 22;
  p = &a;
  printf("%d\n", (int *)p);
  p = &b;
  printf("%d\n", (int *)p);
}
`, '11');

  console.log('\nDone.');
}

main().catch(console.error);
