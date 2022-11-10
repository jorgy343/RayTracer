#![feature(test)]

extern crate test;
extern crate yart;

#[cfg(test)]
mod tests {
    use test::Bencher;

    #[bench]
    fn bench_add_two(b: &mut Bencher) {
        let scene = load_scene();

        b.iter(|| add_two(2));
    }
}
