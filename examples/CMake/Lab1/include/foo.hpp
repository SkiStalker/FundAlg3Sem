class Foo {
   private:
	int _val;

   public:
	explicit Foo(int val);
	~Foo();

	int return_zero() const;
};
