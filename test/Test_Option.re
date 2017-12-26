open Mocha;
open BsJsverify.Verify.Arbitrary;
open BsJsverify.Verify.Property;
open Functors;
let ((<.)) = Function.Infix.((<.));


describe("Option", () => {
  let option_from_tuple: (('a, Js.boolean)) => option('a) = (a) => {
    let (v, b) = a;
    b == Js.true_ ? Some(v) : None;
  };

  describe("Semigroup", () => {
    module V = Verify.Semigroup(OptionF.Int.Additive.Semigroup);
    property3(
      "should satisfy associativity",
      arb_tuple((arb_nat, arb_bool)),
      arb_tuple((arb_nat, arb_bool)),
      arb_tuple((arb_nat, arb_bool)),
      (a, b, c) =>
        V.associativity(option_from_tuple(a), option_from_tuple(b), option_from_tuple(c))
    )
  });

  describe("Monoid", () => {
    module V = Verify.Monoid(OptionF.Int.Additive.Monoid);
    property1(
      "should satisfy neutrality",
      arb_tuple((arb_nat, arb_bool)),
      V.neutral <. option_from_tuple
    )
  });

  describe("Functor", () => {
    module V = Verify.Functor(Option.Functor);
    property1(
      "should satisfy identity",
      arb_tuple((arb_nat, arb_bool)),
      V.identity <. option_from_tuple
    );
    property1(
      "should satisfy composition",
      arb_tuple((arb_nat, arb_bool)),
      V.composition((++)("!"), string_of_int) <. option_from_tuple
    );
  });

  describe("Apply", () => {
    module V = Verify.Apply(Option.Apply);
    property1(
      "should satisfy associative composition",
      arb_tuple((arb_nat, arb_bool)),
      V.associative_composition(Some((++)("!")), Some(string_of_int)) <. option_from_tuple
    )
  });

  describe("Applicative", () => {
    module V = Verify.Applicative(Option.Applicative);
    property1(
      "should satisfy identity",
      arb_tuple((arb_nat, arb_bool)),
      V.identity <. option_from_tuple
    );
    property1("should satisfy homomorphism", arb_nat, V.homomorphism(string_of_int));
    property1("should satisfy interchange", arb_nat, V.interchange(Some(string_of_int)));
  });

  describe("Monad", () => {
    module V = Verify.Monad(Option.Monad);
    let (pure) = Option.Applicative.(pure);

    property1(
      "should satisfy associativity",
      arb_tuple((arb_nat, arb_bool)),
      V.associativity(pure <. string_of_int, pure <. (++)("!")) <. option_from_tuple
    );
    property1(
      "should satisfy left identity", arb_nat, V.left_identity(pure <. string_of_int)
    );
    property1(
      "should satisfy right identity",
      arb_tuple((arb_nat, arb_bool)),
      V.right_identity <. option_from_tuple
    );
  });

  describe("Alt", () => {
    module V = Verify.Alt(Option.Alt);

    property3(
      "should satisfy associativity",
      arb_tuple((arb_nat, arb_bool)),
      arb_tuple((arb_nat, arb_bool)),
      arb_tuple((arb_nat, arb_bool)),
      (a, b, c) =>
        V.associativity(option_from_tuple(a), option_from_tuple(b), option_from_tuple(c))
    );

    property2(
      "should satisfy distributivity",
      arb_tuple((arb_nat, arb_bool)),
      arb_tuple((arb_nat, arb_bool)),
      (a, b) =>
        V.distributivity(string_of_int, option_from_tuple(a), option_from_tuple(b))
    )
  });

  describe("Plus", () => {
    module V = Verify.Plus(Option.Plus);

    it("should satisfy annihalation", () => {
      expect(V.annihalation(string_of_int)).to_be(true);
    });
    property1(
      "should satisfy left identity",
      arb_tuple((arb_nat, arb_bool)),
      V.left_identity <. option_from_tuple
    );
    property1(
      "should satisfy right identity",
      arb_tuple((arb_nat, arb_bool)),
      V.right_identity <. option_from_tuple
    );
  });

  describe("Alternative", () => Option.Alternative.({
    module V = Verify.Alternative(Option.Alternative);

    property1(
      "should satisfy distributivity",
      arb_tuple((arb_nat, arb_bool)),
      V.distributivity(pure((*)(2)), pure((+)(3))) <. option_from_tuple
    );

    it("should satisfy annihalation", () => {
      expect(V.annihalation(string_of_int |> pure)).to_be(true)
    })
  }));

  describe("Foldable", () => {
    let (fold_left, fold_right) = Option.Foldable.(fold_left, fold_right);

    it("should do a left fold", () => {
      expect(fold_left((+), 0, Some(1))).to_be(1);
    });
    it("should do a right fold", () => {
      expect(fold_right((+), 0, Some(1))).to_be(1);
      expect(fold_right((+), 0, None)).to_be(0);
    });
    it("should do a map fold (int)", () => {
      let fold_map = OptionF.Int.Additive.Fold_Map.(fold_map);
      expect(fold_map((*)(2), Some(3))).to_be(6);
      expect(fold_map((+)(1), None)).to_be(Int.Additive.Monoid.empty);
    });
    it("should do a map fold (list)", () => {
      let fold_map = OptionF.List.Fold_Map_Plus.(fold_map);
      expect(fold_map(List.Applicative.pure, Some(123))).to_be([123]);
    });
  });

  describe("Traversable", () => {
    let (traverse, sequence) = OptionF.List.Traversable.(traverse, sequence);

    it("should traverse the list", () => {
      let positive_int = (x) => x >= 0 ? [x] : [];
      expect(traverse(positive_int, Some(123))).to_be([Some(123)]);
      expect(traverse(positive_int, Some(-123))).to_be([]);
    });
    it("should sequence the list", () => {
      expect(sequence(Some([3, 4, 5]))).to_be([Some(3), Some(4), Some(5)]);
      expect(sequence(None)).to_be([None]);
    });
  });

  describe("Eq", () => {
    module V = Verify.Eq1(OptionF.Int.Eq);

    property1(
      "should satisfy reflexivity",
      arb_tuple((arb_nat, arb_bool)),
      V.reflexivity <. option_from_tuple
    );
    property2(
      "should satisfy symmetry",
      arb_tuple((arb_nat, arb_bool)),
      arb_tuple((arb_nat, arb_bool)),
      (a, b) =>
        V.symmetry(option_from_tuple(a), option_from_tuple(b))
    );
    property3(
      "should satisfy transitivity",
      arb_tuple((arb_nat, arb_bool)),
      arb_tuple((arb_nat, arb_bool)),
      arb_tuple((arb_nat, arb_bool)),
      (a, b, c) =>
        V.transitivity(option_from_tuple(a), option_from_tuple(b), option_from_tuple(c))
    );
  });
});