#include "regExp.cpp"
#include <stdexcept>


// shared_ptr<Val> inj(shared_ptr<REGEX> r,char c, shared_ptr<Val> v){
//     v.inj(r,c);

//   case (STAR(r), Sequ(v1, Stars(vs))) => Stars(inj(r, c, v1)::vs)
//   case (SEQ(r1, r2), Sequ(v1, v2)) => Sequ(inj(r1, c, v1), v2)
//   case (SEQ(r1, r2), Left(Sequ(v1, v2))) => Sequ(inj(r1, c, v1), v2)
//   case (SEQ(r1, r2), Right(v2)) => Sequ(mkeps(r1), inj(r2, c, v2))
//   case (ALT(r1, r2), Left(v1)) => Left(inj(r1, c, v1))
//   case (ALT(r1, r2), Right(v2)) => Right(inj(r2, c, v2))
//   case (CHAR(d), Empty) => Chr(c) 
//   case (RECD(x, r1), _) => Rec(x, inj(r1, c, v))
//   case (RANGE(set), Empty) => Chr(c)
//   case (PLUS(r1), Sequ(v, Stars(values))) => Plus(inj(r1, c, v)::values)
//   case (OPTIONAL(r1), _) => Optional(inj(r1, c, v))
//   case (NTIMES(r1,_), Sequ(r, Ntimes(cs))) => Ntimes(inj(r1, c, r)::cs)
// }

// class Sequ : public Val {
//  public:
//   Sequ(Val* v1, Val* v2) : v1_(v1), v2_(v2) {}
//   Val* v1_;
//   Val* v2_;
//   Val* inj(char c, Rexp* r) {
//     return new Sequ(v1_->inj(c, r), v2_->inj(c, r));
//   }
// };



// shared_ptr<Val> lex_simp(shared_ptr<REGEX> r, string & s,int i){
//     int i = 0;
//     if(i >= s.size()){
//         if(r->nullable()){
//             return r->mkeps();
//         }
//         else{
//             throw std::invalid_argument("lexing error");
//         }
//     }
//     shared_ptr<REGEX> exp = r->der(s[i])->simp();
//     return inj(r,s[i],lex_simp(r,s,++i));
    
// }

// def lex_simp(r: Rexp, s: List[Char]) : Val = s match {
//   case Nil => if (nullable(r)) mkeps(r) else 
//     { throw new Exception("lexing error") } 
//   case c::cs => {
//     val (r_simp, f_simp) = simp(der(c, r))
//     inj(r, c, f_simp(lex_simp(r_simp, cs)))
//   }
// }

//var taken in a regeular expression and returns a var

// def inj(r: Rexp, c: Char, v: Val) : Val = (r, v) match {
//   case (STAR(r), Sequ(v1, Stars(vs))) => Stars(inj(r, c, v1)::vs)
//   case (SEQ(r1, r2), Sequ(v1, v2)) => Sequ(inj(r1, c, v1), v2)
//   case (SEQ(r1, r2), Left(Sequ(v1, v2))) => Sequ(inj(r1, c, v1), v2)
//   case (SEQ(r1, r2), Right(v2)) => Sequ(mkeps(r1), inj(r2, c, v2))
//   case (ALT(r1, r2), Left(v1)) => Left(inj(r1, c, v1))
//   case (ALT(r1, r2), Right(v2)) => Right(inj(r2, c, v2))
//   case (CHAR(d), Empty) => Chr(c) 
//   case (RECD(x, r1), _) => Rec(x, inj(r1, c, v))
//   case (RANGE(set), Empty) => Chr(c)
//   case (PLUS(r1), Sequ(v, Stars(values))) => Plus(inj(r1, c, v)::values)
//   case (OPTIONAL(r1), _) => Optional(inj(r1, c, v))
//   case (NTIMES(r1,_), Sequ(r, Ntimes(cs))) => Ntimes(inj(r1, c, r)::cs)
// }