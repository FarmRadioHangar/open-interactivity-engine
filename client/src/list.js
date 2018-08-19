export default class List {

  constructor(generator, length = Infinity) {
    this[Symbol.iterator] = generator;
    this.length = length;
  }

  static enumFrom(n) {
    return new List(function* () {
      while (true) {
        yield n;
        n += 1;
      }
    });
  }

  static nats() {
    return this.enumFrom(0);
  }

  map(fun) {
    const generator = this[Symbol.iterator];
    return new List(function* () {
      for (const val of generator()) {
        yield fun(val);
      }
    }, this.length);
  }

  take(n) {
    const generator = this[Symbol.iterator];
    return new List(function* () {
      let it = generator(),
          next = it.next(),
          i = 0;
      while (!next.done && i < n) {
        yield next.value;
        next = it.next();
        ++i;
      }
    }, Math.min(this.length, n));
  }

  toArray() {
    return [...this];
  }

}
