package main

union myUnion {
  var a: int,
  var b: float,
  var c: double
}

func sum(a: int, b: int) -> int {
  return a + b
}

func main() {
  var hello_world: string = "HELLO WORLD!!!"
  var condition0: bool = false
  condition1 := true
  real := .111

  if (condition0 && condition1) {
    println(hello_world)
  } else if (condition0 || condition1) {
    for _ := range 5 {
      println("HI!")
    }
  }

  return 0
}
