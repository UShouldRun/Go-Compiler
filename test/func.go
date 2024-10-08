package main

union myUnion {
  var a: int,
  var b: float,
  var c: double
}

func sum(a: int, b: int) -> int {
  return a + b
}

func bit(a: int, b: int) -> int {
  return a & b
}

func xor(a: int, b: int) -> int {
  return a | b
}

func main() {
  var hello_world: string = "HELLO WORLD!!!"
  var condition0: bool = false
  var count: uint32 = 32
  condition1 := true
  r1 := .111
  r2 := &r1

  for _, x range count {
    r1 &= r2
    r2 /= r1
    r2 |= r1
    x += r1 ^ r2
  }

  if (condition0 && condition1) {
    println(hello_world)
  } else if (condition0 || condition1) {
    for _ := range 5 {
      println("HI!")
    }
  }

  return 0
}
