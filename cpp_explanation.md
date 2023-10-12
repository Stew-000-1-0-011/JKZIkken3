# はじめに
C++は悪い言語である。
しかし、現在主流の多くの言語よりかマシな場合も多い。
少なくともCよりははるかにマシだ。
言語機能的にJavaに劣る点はほぼ無いと言って差し支えないかと思う。

この授業はC++に親しむ良い機会だ。
そこで、ここではC++の主要機能の一部を説明する。

なお、公認サークルCRS、および千葉大公認コミュニティCCHではC++の講座を行う。
興味のある方はぜひ受講し、なんなら講師にヤジを飛ばしてほしい。

# TOC
いい感じのビューワーを使えば勝手に表示してくれよう。割愛。

# namespace 名前空間
名前をぽこじゃかグローバルスコープに宣言すると、いずれ名前が被る。
Cなどで意図しない名前と被って気味の悪いエラーに遭遇したことはないだろうか。(参考: [windows.hを入れると`min`という識別子が使えなくなる](https://yohhoy.hatenadiary.jp/entry/20120115/p1))

これを防ぐために、グローバルスコープ内の名前を階層化する。
この機能を名前空間と呼ぶ。

## 文法
名前空間内で何かを宣言するときは次のように書く：
```c++
namespace 名前空間名 {
	// 関数、変数、型その他の宣言...
}
```
```c++
// example
namespace my_library {
	void my_function(int x) {
		// ...
	}

	int my_variable = 42;
}
```

名前を使用するほうはこんな感じ：
```c++
名前空間名::名前
```
```c++
// example
int foo = my_library::my_variable;
my_function(foo);
```

注：
「名前空間名::名前」も名前だ。名前空間名::を含まない名前を指してunqualified nameと呼ぶ。

ところで、階層的なものの代表例はファイルパスだろう。
ファイルパスは相対的に書ける。同様に、名前も相対的に書ける。

同じディレクトリ直下にあるファイルをファイル名のみで指定できるように、
同じ名前空間にあるunqualified nameは名前空間名を省略して書ける。
```c++
// example
namespace my_library {
	int my_variable = 42;

	void my_function(int x) {
		// don't need to write my_library::my_variable
		my_variable = x;
	}
}
```

# function template 関数テンプレート
テンプレートの一種。関数を作るテンプレートである。

テンプレート。これこそはC++の最たる魅力である。
コンパイル時に定まる情報を与え、型、関数、変数を生成するものだ。
コンパイル時に定まる情報とは、型や定数値である。
例えば
```c++
template<class T1, class T2, int n>
struct MyStructTemplate {
	T1 var1;
	T2 var2;
	T1 arr[n];
};
```
というテンプレートは、
$$
MyTemplate: (T1: type, T2: type, n: int, p: bool) \to type, \\
MyTemplate(T1, T2, n, p) = type \{var1: T1, var2: T2, arr: T1[n]\}
$$
という写像のようにイメージできる。

これによりC++はサブタイピングによる多相性(継承を使うやつ)よりも強力な多相性を実現している。
サブタイピングによる多相性は、その型を使う側に同一の記述による型の使用を可能とさせる。
一方、テンプレートなどのパラメトリック多相性は、その型を**定義する側にも**同一の記述による型の定義を可能とさせる。


以降、copilot任せに英語で書いたままです、手が空いたらちゃんと書きます。

# final
`final` is a keyword to prevent inheritance.
Nothing to explain. Inheritance is evil.

# function specifier 関数指定子
## inline
`inline` is a "hint" to compiler to apply inline expansion(one way of optimization).
`inline` also has another meaning: it allows multiple definition of function in multiple translation unit.
(I use this meaning here, but you don't have to care about this.)

## constexpr
`constexpr` is a "hint" to compiler to evaluate this function at compile time.
we should use `constexpr` whenever possible: Constexpr All The Things!

# trailing return type 後置戻り値型
```bnf
auto <func-name>(<parameter-list>) -> <return-type>
```
is a syntax of function declaration.

# noexcept
`noexcept` is explicit exception specification. Don't worry about this.

# コンセプト concept
コンセプトは、テンプレートの型パラメータに制約を与えるための機能である。
詳細はhttps://cpprefjp.github.io/lang/cpp20/concepts.html を参照。