/*! \file fl.cpp
 * fl in the program name is an acronym for Feature Location.
 * \author Ulrich W. Eisenecker
 * \date February 4, 2023
 * \warning This program is a research prototype only. It comes with
 *  no warranty and no liability. Use it only at your own risk!
 * \copyright This program is released under the Apache License 2.0.
 */

#include <fstream> // because of ofstream
#include <iostream>
#include <vector> // because of vector<bool> and vector<>
#include <exception>
using namespace std;

/*! checking turns some checks for errors on or off.
 *  Initializing checking to true, turns on error checking.
 *  Initializing checking to false, turns off error checking.
 *  checking should always be tested wit hif constexpr.
 *  Only then, the code performing the actual check 
 *  will not be included in the object code
 *  if checking is initialized to false.
 */
constexpr bool checking { true }; // Initialize to false
                                  // to turn off runtime checking.
/*! Type alias for the program specific name for the
 *  unsigned integer type to be used throughout the program.
 */
using maxnat_t = uintmax_t;

/*! This string constant defines the prefix that is
 *  used consistently for the names of all files that are
 *  produced by the program.
 */
const string prefix { "fl_" };
/*! Calculates and returns the power of base and exponent.
 * \param base Value for the base of the power expression.
 * \param exponent Value for the exponent of the power expression.
 * \returns Value of power.
 */
maxnat_t power(maxnat_t base,maxnat_t exponent)
{
   maxnat_t result { 1llu }; // 1llu is specific for uintmax_t
   while (exponent--)
   {
      result *= base;
   }
   return result;
}

/*! Returns the result of integral division of x by y rounded
 * to the nearest upper integer value.
 * The following code was taken from 
 * https://stackoverflow.com/questions/2745074/fast-ceiling-of-an-integer-division-in-c-c ,
 * Answer 3
 *\param x Dividend.
 *\param y Divisor.
 *\returns Result of integral division rounded to the nearest upper integer.
 */
maxnat_t ceil_div(const maxnat_t& x,const maxnat_t& y)
{
   return x / y + !!(x % y);
}

/*! Type alias for the program specific name for the
 *  unsigned integer type that is used consistently
 *  for everything related to the combination_t class. 
 */
using combination_element_t = unsigned short int;

/*! Exemplars of this class allow to systematically generate
 *  all combinations for n elements and sample size k.
 */
class combination_t
{
   public:
      /*! A combination exemplar must be initialized with 
       *  number of elements and sample size.
       *\param n Number of elements.
       *\param k Sample size.
       */
      combination_t(combination_element_t n,combination_element_t k)
         :m_n(n),m_k(k)
      {
         if constexpr (checking)
         { 
            if (k > n)
            {
               throw domain_error("k > n");
            }
            if (n == 0)
            {
               throw domain_error("n == 0");
            }
         }
         initialize();
      }
      /*! Creates initial valid state for a combination_t exemplar.
       */
      void initialize()
      {
         combination_element_t start { };
         for (auto i { k() }; i > 0; --i)
         {
            m_state.push_back(start++);
         }
      };
      /*! Returns sample size.
       *\returns Sample size.
       */
      combination_element_t k() const
      {
         return m_k;
      }
      /*! Returns number of elements.
       *\returns Number of elements.
       */
      combination_element_t n() const
      {
         return m_n;
      }
      /*! Tries to generate next combination.
       *\returns True, if next combination is available, and false otherwise.
       */
      bool next()
      {
         for (auto i { m_state.size() }; i > 0; --i)
         {
            auto v { m_state.at(i - 1) };
            if (v < m_n && v + 1 + k() - i < m_n)
            {
              m_state.at(i - 1) += 1;
              for (auto j { i }; j < m_state.size(); ++j)
              {
                 m_state.at(j) = m_state.at(j - 1) + 1;
              }
              return true;
            }
         }
         return false;
      }
      /*! Returns vector that containes the elements of the actual combination.
       *\returns Copy of internal state of combination as vector.
       */
      std::vector<combination_element_t> operator()() const
      {
        return m_state;
      }
   private:
      std::vector<combination_element_t> m_state;
      combination_element_t m_k;
      combination_element_t m_n;
};

/*! Type alias for the program specific name for a
 *  difference expression to be used throughout the program.
 */
using difference_expression_t = vector<bool>;

/*! Calculates and returns bitwise negation of difference expression.
 *\param de Difference expression passed by value.
 *\returns Negated difference expression as value.
 */
difference_expression_t operator~(difference_expression_t de)
{
   // vector<bool> seems to lack support for range-based for loops.
   for (maxnat_t i { 0 }; i < de.size() ; ++i)
   {
      de[i] = !de[i]; // The elements of vector<bool> are bool's.
   }                  // Therefore, ! should be used for negation.
   return de;
}

/*! Performs bitwise-and assignment for difference expressions.
 *\param left Difference expression passed as reference.
 *\param right Difference expression passed as reference to const.
 *\returns Difference expression left that has been bitwise-and-assigned with right as reference.
 */
difference_expression_t& operator&=(difference_expression_t& left,
                                    const difference_expression_t& right)
{
   if constexpr (checking)
   {
      if (left.size() != right.size())
      {
         throw length_error("left.size() != right.size()");
      }
   }
   for (maxnat_t i { 0 }; i < left.size() ; ++i)
   {
      left[i] = left[i] && right[i];
   }
   return left;
}

/*! Performs bitwise-or assignment for difference expressions.
 *\param left Difference expression passed as reference.
 *\param right Difference expression passed as reference to const.
 *\returns Difference expression left that has been bitwise-or-assigned with right as reference.
 */
difference_expression_t& operator|=(difference_expression_t& left,
                                    const difference_expression_t& right)
{
   if constexpr (checking)
   {
      if (left.size() != right.size())
      {
         throw length_error("left.size() != right.size()");
      }
   }
   for (maxnat_t i { 0 }; i < left.size() ; ++i)
   {
      left[i] = left[i] || right[i];
   }
   return left;
}

/*! Stream insertion operator for difference expressions.
 *\param os Output stream passed as reference.
 *\param de Difference expression passed as reference to const.
 *\returns Output stream as reference.
 */
ostream& operator<<(ostream& os,const difference_expression_t& de)
{
   for (maxnat_t i { de.size() }; i > 0; --i)
   {
      os << de[i - 1];
   }
   return os;
}

/*! Exemplars of this class allow to systematically generate
 *  all difference expressions for isolating independent features
 *  for given number F of independent features.
 */
class difference_expression_generator
{
   public:
      /*! A difference expression exemplar must be initialized with 
       *  the number of independent features.
       *\param F Number of independent features.
       */
      explicit difference_expression_generator(const maxnat_t F):m_F { F },
                                                                 m_S { power(2,F) }
      {}
      /*! Calculates and returns difference expression for given feature.
       *\param f Feature-id (1..F)
       *\returns Difference expression as value.
       */
      difference_expression_t operator()(maxnat_t f) const
      {
         if constexpr (checking)
         {
            if (f > F())
            {
               throw invalid_argument("f > F()");
            }
         }
         difference_expression_t bitstring { };
         bitstring.reserve(S());
         maxnat_t counter { 0 };
         bool bit { false };
         maxnat_t stride { power(2,f - 1) };
         for (maxnat_t system { 0 }; system < S(); ++system)
         {
            bitstring.push_back(bit);
            ++counter;
            if (counter == stride)
            {
               bit = !bit;
               counter = 0;
            }
         }
         return bitstring;
      }
      /*! Calculates and returns value for given feature and system.
       * Important: The implementation calculates this value arithmetically.
       * It does not create a difference expression.
       * Therefore, it requires only minimal memory.
       *\param f Feature-id (1..F)
       *\param s System-id (1..S)
       *\returns True if s is to be intersected (I-part) and false if it is to be united (U-part) in corresponding difference expression.
       */
      bool operator()(maxnat_t f,maxnat_t s) const
      {
         if constexpr (checking)
         {
            if (f > F())
            {
               throw invalid_argument("f > F()");
            }
            if (s > S())
            {
               throw invalid_argument("s > S()");
            }
         }
         maxnat_t stride { power(2,f - 1) };
         return !(ceil_div(s,stride) % 2);
      }
      /*! Returns number of independent features.
       *\returns Number of independent features F.
       */
      const maxnat_t F() const
      {
         return m_F;
      }
      /*! Returns number of systems.
       *\returns Number ofsystems S.
       */
      const maxnat_t S() const
      {
         return m_S;
      }
   private:
      const maxnat_t m_F;
      const maxnat_t m_S;
};

/*! Outputs difference expressions for independent features to a file.
 * This implementation creates and uses difference expressions.
 *\param dg Difference generator to be used for generating difference expressions.
 */
void print_independent_features(const difference_expression_generator& dg)
{
   ofstream os { prefix + to_string(dg.F()) + "_F.csv" };

   for (maxnat_t f { 1 };f <= dg.F(); ++f)
   {
      os << 'f' << f << '\t' << dg(f) << endl;
   }
}

/*! Outputs difference expressions for independent features to a file.
 * This implementation does neither create nor uses difference expressions!
 * Instead, the value that indicates if a system is to be intersected or to be
 * united is calculated for each individual system.
 * Therefore, this function has minimal memory requirements.
 * In general, it is possible to provide corresponding implementations
 * for all other following functions whose names begin with print_.
 *\param dg Difference generator to be used for generating difference expressions.
 */
void print_independent_features_alt(const difference_expression_generator& dg)
{
   ofstream os { prefix + to_string(dg.F()) + "_F.csv" };

   for (maxnat_t f { 1 };f <= dg.F(); ++f)
   {
      os << 'f' << f << '\t';
      for (maxnat_t s { dg.S() }; s > 1; --s)
      {
         os << dg(f,s);
      }
      os << endl;
   }
}

/*! Outputs difference expressions for not features to a file.
 * This implementation creates and uses difference expressions.
 *\param dg Difference generator to be used for generating difference expressions.
 */
void print_not_features(const difference_expression_generator& dg)
{
   ofstream os {prefix + to_string(dg.F()) + "_N.csv" };

   for (maxnat_t f { 1 };f <= dg.F(); ++f)
   {
      os << "!f" << f << '\t' << ~dg(f) << endl;
   }
}

/*! Outputs difference expressions for and features to a file.
 * This implementation creates and uses difference expressions.
 *\param dg Difference generator to be used for generating difference expressions.
 */
void print_and_features(const difference_expression_generator& dg)
{
   ofstream os { prefix + to_string(dg.F()) + "_A.csv" };

   for (maxnat_t f { 2 }; f <= dg.F(); ++f)
   {
      combination_t c(dg.F(),f);
      do
      {
         auto features { c() };
         auto result { dg(features.at(0) + 1) };
         string i { "f"s + to_string(features.at(0) + 1) };
         for (maxnat_t e { 1 }; e < features.size(); ++ e)
         {
            result &= dg(features.at(e) + 1);
            i += ("*f"s + to_string(features.at(e) + 1));
         }
         os << i << '\t' << result << endl;
      } while (c.next());
   }
}

/*! Outputs difference expressions for or features to a file.
 * This implementation creates and uses difference expressions.
 *\param dg Difference generator to be used for generating difference expressions.
 */
void print_or_features(const difference_expression_generator& dg)
{
   ofstream os { prefix + to_string(dg.F()) + "_O.csv" };

   for (maxnat_t f { 2 }; f <= dg.F(); ++f)
   {
      combination_t c(dg.F(),f);
      do
      {
         auto features { c() };
         auto result { dg(features.at(0) + 1) };
         string i { "f"s + to_string(features.at(0) + 1) };
         for (maxnat_t e { 1 }; e < features.size(); ++ e)
         {
            result |= dg(features.at(e) + 1);
            i += ("+f"s + to_string(features.at(e) + 1));
         }
         os << i << '\t' << result << endl;
      } while (c.next());
   }
}

/*! Outputs difference expressions for and-not features to a file.
 * This implementation creates and uses difference expressions.
 *\param dg Difference generator to be used for generating difference expressions.
 */
void print_and_not_features(const difference_expression_generator& dg)
{
   ofstream os { prefix + to_string(dg.F()) + "_AN.csv" };

   for (maxnat_t f { 2 }; f <= dg.F(); ++f)
   {
      combination_t c(dg.F(),f);
      do
      {
         auto features { c() };
         auto result { ~dg(features.at(0) + 1) };
         string i { "!f"s + to_string(features.at(0) + 1) };
         for (maxnat_t e { 1 }; e < features.size(); ++ e)
         {
            result &= ~dg(features.at(e) + 1);
            i += ("*!f"s + to_string(features.at(e) + 1));
         }
         os << i << '\t' << result << endl;
      } while (c.next());
   }
}

/*! Outputs difference expressions for or-not features to a file.
 * This implementation creates and uses difference expressions.
 *\param dg Difference generator to be used for generating difference expressions.
 */
void print_or_not_features(const difference_expression_generator& dg)
{
   ofstream os { prefix + to_string(dg.F()) + "_ON.csv" };

   for (maxnat_t f { 2 }; f <= dg.F(); ++f)
   {
      combination_t c(dg.F(),f);
      do
      {
         auto features { c() };
         auto result { ~dg(features.at(0) + 1) };
         string i { "!f"s + to_string(features.at(0) + 1) };
         for (maxnat_t e { 1 }; e < features.size(); ++ e)
         {
            result |= ~dg(features.at(e) + 1);
            i += ("+!f"s + to_string(features.at(e) + 1));
         }
         os << i << '\t' << result << endl;
      } while (c.next());
   }
}

int main()
{
   maxnat_t F { };
   cout << "F = " << flush;
   cin >> F;

   difference_expression_generator dg(F);
   // If you uncomment the next line, you should de-comment the over-next line
   print_independent_features(dg);
//   print_independent_features_alt(dg);
   print_not_features(dg);
   print_and_features(dg);
   print_or_features(dg);
   print_and_not_features(dg);
   print_or_not_features(dg);
}
