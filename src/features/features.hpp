/*! \file features.hpp
 *
 * \author Ulrich W. Eisenecker
 * \date December 11, 2022
 * \warning This program is a research prototype only. It comes with
 *  no warranty and no liability. Use it only at your own risk!
 * \copyright This program is released under the Apache License 2.0.
 */

#ifndef FEATURES_H
#define FEATURES_H

#include <string>
#include <vector>
#include <map>

#include <set>
#include <algorithm>
#include <exception>
#include <cstddef>
#include <iostream>
#include <climits>
#include <cstdint> // because of std::uintmax_t

/*! This namespace contains all types and functions that are related
    to feature location.
 */
namespace features
{
   const std::string set_begin { "{" };
   const std::string set_end { "}" };
   const std::string set_intersection { "&" };
   const std::string set_difference { "\\" };
   const std::string set_union { "|" };
   const std::string set_separator { " " };  
   const std::string set_comma { "," };
   const std::string feature_and { "*" };
   const std::string feature_or { "+" };
   const std::string feature_not { "!" };
   const std::string feature { "f" };
   const std::string feature_separator { " " };
   const std::string system { "S" };
   const std::string difference_expression { "E" };
   const std::string separator { "\t" };
   const std::string opening_parenthesis { "(" };
   const std::string closing_parenthesis { ")" };

   /*! Type alias for a feature ID.
    */
   using feature_id_t  = unsigned short int;
   /*! Type alias for a model ID.
    */
   using model_id_t    = unsigned short int;
   /*! Type alias for largest natural number type.
    */
   using maxnat_t      = uintmax_t;

   /*! Type alias for collection of feature names and associated
       set difference IDs.
    */
   using feature_expression_t = std::vector<std::pair<std::string,maxnat_t>>;
   /*! Type alias for mapping a set difference ID to a feature name.
    */
   using expression_feature_t = std::map<maxnat_t,std::string>;
   /*! Type alias for a collection of feature names.
    */
   using feature_names_t      = std::vector<std::string>;
   /*! Type alias for the feature names that define a system.
    */
   using system_t             = feature_names_t;
   /*! Type alias for a collection of systems that are defined
       by their feature names.
    */
   using systems_t            = std::vector<system_t>;
   
   /*! Represents a single set difference expression of systems.
    */
   struct systems_difference_t
   {
      /*! Names of the systems with features that must be intersected,
          that is, the left operand of the set difference.
       */
      std::set<std::string> intersections,
      /*! Names of the system with features that must be united,
          that is, the right operand of the set difference.
       */
                            unions;
   };

   /*! Represents a triple containing the set difference ID,
       the name of the string to be isolated, and the actual
       set difference of systems that is required.
    */
   struct system_feature_difference_t
   {
      /*! Set difference ID.
       */
      maxnat_t difference_id;
      /*! Name of the feature to be isolated.
       */
      std::string feature;
      /*! Actual set difference that isolates the corresponding feature.
       */
      systems_difference_t difference;
   };

   using features_isolation_t = std::map<std::string,systems_difference_t>;
   using differences_t = std::vector<system_feature_difference_t>;

   /*! Returns product of numbers in the range [from,to].
    */
   maxnat_t product(maxnat_t from,maxnat_t to)
   {
      if (from == 0llu || to == 0llu)
         return 0llu;
      maxnat_t result { 1llu };
      while (from <= to)
      {
         result *= to--;
      }
      return result;
   }

   /*! Returns factorial of n.
    */
   maxnat_t factorial(maxnat_t n)
   {
      return n == 0llu ? 1llu : product(1llu,n);
   }

   /*! Returns number of combinations of n items and sample size k.
    */
   maxnat_t combinations(maxnat_t n,maxnat_t k)
   {
      return product(n + 1llu - k,n) / factorial(k);
   }

   /*! Returns sum of combinations of n items in the sample range [k,n].
    */
   maxnat_t sum_of_combinations(maxnat_t n,maxnat_t k)
   {
      maxnat_t result { 0llu };
      while (k <= n)
      {
       result += combinations(n,k++);
      } 
      return result;
   }

   /*! Returns base to the power of exponent.
    */
   maxnat_t power(maxnat_t base,maxnat_t exponent)
   {
      maxnat_t result { 1llu };
      while (exponent--)
      {
         result *= base;
      }
      return result;
   }

   /*! Returns 2 to the power of exponent.
    */
   maxnat_t power2(maxnat_t exponent)
   {
      return power(2,exponent);
   }

   /*! Function template that returns vector destination after appending vector v.
    */
   template <class T>
   std::vector<T>& concat(std::vector<T>& destination,const std::vector<T>& v)
   {
      destination.reserve(destination.size() + v.size());
      destination.insert(destination.end(),v.cbegin(),v.cend());
      return destination;
   }

   /*! Function template that returns true if vector v contains element e, and false otherwise.
    */
   template <class T>
   bool contains(const std::vector<T>& v,const T& e)
   {
      return std::find(v.cbegin(),v.cend(),e) != v.cend();

   }

   /*! Function template that returns a vector that results from negating vector v
       with respect to the range [1,n].
       For example: { 1 }, 3 --> { 2, 3 }
   */
   std::vector<feature_id_t> negate(const std::vector<feature_id_t>& v,feature_id_t n)
   {
      std::vector<feature_id_t> result;
      for (feature_id_t i { 1 }; i <= n; ++i)
      {
         if (!contains(v,i))
         {
            result.push_back(i);
         }
      }
      return result;
   }

   /*! Returns true if model M has or-features and false otherwise.
   */
   bool hasO(model_id_t M)
   {
      return (M ==  2 || M ==  4 || M ==  6 || M ==  8 || M == 11 || M == 13 ||
              M == 14 || M == 16 || M == 17 || M == 19
             );
   }

   /*! Returns true if model M has and-features and false otherwise.
   */
   bool hasA(model_id_t M)
   {
      return (M ==  3 || M ==  4 || M ==  7 || M ==  8 || M == 12 || M == 13 ||
              M == 15 || M == 16 || M == 18 || M == 19
             );
   }

   /*! Returns true if model M has not-features and false otherwise.
   */
   bool hasN(model_id_t M)
   {
      return (M ==  5 || M ==  6 || M ==  7 || M ==  8 || M ==  9 || M == 10 ||
              M == 11 || M == 12 || M == 13 || M == 14 || M == 15 || 
              M == 16 || M == 17 || M == 18 || M == 19
             );
   }

   /*! Returns true if model M has or-not-features and false otherwise.
   */
   bool hasON(model_id_t M)
   {
      return (M ==  9 || M == 11 || M == 12 || M == 13 || M == 17 || 
              M == 18 || M == 19
             );
   }

   /*! Returns true if model M has and-not-features and false otherwise.
   */
   bool hasAN(model_id_t M)
   {
      return (M == 10 || M == 14 || M == 15 || M == 16 || M == 17 ||
              M == 18 || M == 19
             );
   }

   /*! Returns name for independent feature with id i.
   */
   std::string independent_feature_name(feature_id_t i)
   {
      return feature + std::to_string(i); 
   }

   /*! Returns name for or-feature of feature ids.
   */
   std::string or_feature_name(const std::vector<feature_id_t>& ids)
   {
      std::string result { feature + std::to_string(ids.at(0)) };
      for (std::size_t i { 1 }; i < ids.size(); ++i)
      {
         result = result + feature_separator + feature_or + feature_separator
                         + feature 
                         + std::to_string(ids.at(i));
      }
      return result;
   }

   /*! Returns name for and-feature of feature ids.
   */
   std::string and_feature_name(const std::vector<feature_id_t>& ids)
   {
      std::string result { feature + std::to_string(ids.at(0)) };
      for (std::size_t i { 1 }; i < ids.size(); ++i)
      {
         result = result + feature_separator + feature_and + feature_separator
                         + feature
                         + std::to_string(ids.at(i));
      }
      return result;
   }

   /*! Returns name for not-feature with id i.
   */
   std::string not_feature_name(feature_id_t i)
   {
      return feature_not + feature + std::to_string(i); 
   }

   /*! Returns name for or-not-feature of feature ids.
   */
   std::string or_not_feature_name(const std::vector<feature_id_t>& ids)
   {
      std::string result { feature_not + feature + 
                           std::to_string(ids.at(0)) };
      for (std::size_t i { 1 }; i < ids.size(); ++i)
      {
         result = result + feature_separator + feature_or + feature_separator
                         + feature_not 
                         + feature + std::to_string(ids.at(i));
      }
      return result;
   }

   /*! Returns name for and-not-feature of feature ids.
   */
  std::string and_not_feature_name(const std::vector<feature_id_t>& ids)
   {
      std::string result { feature_not + feature +
                           std::to_string(ids.at(0)) };
      for (std::size_t i { 1 }; i < ids.size(); ++i)
      {
         result = result + feature_separator + feature_and + feature_separator
                         + feature_not 
                         + feature + std::to_string(ids.at(i));
      }
      return result;
   }

   /*! Returns name for system with id n.
   */
   std::string system_name(feature_id_t n)
   {
      return system + std::to_string(n);
   }

   /*! Returns name for system set difference expression with id n.
   */
   std::string difference_name(feature_id_t n)
   {
      return difference_expression + std::to_string(n);
   }

   /*! Returns value of or-feature for ids and feature expression idf.
   */
   maxnat_t or_feature_value(const std::vector<feature_id_t>& ids,const feature_expression_t& idf)
   {
      maxnat_t result { 0 };
      for (const auto& v : ids)
      {
         result |= idf.at(v - 1).second;
      }
      return result;
   }

   /*! Returns value of and-feature for ids and feature expression idf.
   */
   maxnat_t and_feature_value(const std::vector<feature_id_t>& ids,const feature_expression_t& idf)
   {
      maxnat_t result { ~0ull };
      for (const auto& v : ids)
      {
         result &= idf.at(v - 1).second;
      }
      return result;
   }

   /*! Returns value of or-not-feature for ids and feature expression nf.
   */
   maxnat_t or_not_feature_value(const std::vector<feature_id_t>& ids,const feature_expression_t& nf)
   {
      maxnat_t result { 0 };
      for (const auto& v : ids)
      {
         result |= nf.at(v - 1).second;
      }
      return result;
   }

   /*! Returns value of and-not-feature for ids and feature expression nf.
   */
   maxnat_t and_not_feature_value(const std::vector<feature_id_t>& ids,const feature_expression_t& nf,const maxnat_t& bitmask)
   {
      maxnat_t result { ~bitmask };
      for (const auto& v : ids)
      {
         result &= nf.at(v - 1).second;
      }
      return result;
   }

   /*! Returns vector that corresponds to the bit pattern of u.
   */
   std::vector<feature_id_t> unsigned2vector(maxnat_t u)
   {
      std::vector<feature_id_t> result;
      for (maxnat_t bit { 0 }; bit < sizeof(u) * CHAR_BIT; ++bit)
      {
         if (u & 1ull << bit)
         {
            result.push_back(bit + 1);
         }
      }
      return result;
   }

   /*! Class template that generates all combinations of size k for symbols.
   */
   template <class T>
   class combination_t
   {
      public:
         /*! Constructor that accepts a vector with symbols and size k.
         */
         combination_t(const std::vector<T> symbols,std::size_t k)
            :m_symbols(symbols),m_k(k)
         {
            if (k > symbols.size())
            {
               throw std::logic_error("n = " + 
                                      std::to_string(symbols.size()) + 
                                      ", k = " + std::to_string(k) +
                                      " violates n >= k!\n"
                                     );
            }
            initialize();
         }
         /*! Member function for initializing a combination.
         */
         void initialize()
         {
            std::size_t start { };
            for (auto i { k() }; i > 0; --i)
            {
               m_state.push_back(start++);
            }
         };
         /*! Returns the size of the combination.
         */
         std::size_t k() const
         {
            return m_k;
         }
         /*! Returns true, if a succeeding state is available for the combination.
         */
         bool next()
         {
            for (auto i { m_state.size() }; i > 0; --i)
            {
               auto v { m_state.at(i - 1) };
               if (v < m_symbols.size() &&
                   v + 1 + k() - i < m_symbols.size() 
                  )
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
         /*! Function call operator that returns a vector containing the
             symbols of the actual combination.
         */
         std::vector<T> operator()() const
         {
           std::vector<T> result;
           for (const auto& e : m_state)
           {
              result.push_back(m_symbols.at(e));
           }
           return result;
         }
      private:
         std::vector<T> m_symbols;
         std::vector <std::size_t> m_state;
         std::size_t m_k;
   };

   /*! Base class for all classes that perform feature location.
    */
   class feature_location_t
   {
      public:
         /*! Constructor that requires the number of idependent features
             and the number of the model as parameters.
          */
         feature_location_t(feature_id_t n_,
                            feature_id_t m_)
              :m_n { n_ },
               m_F { n_ },
               m_M { m_ },
               m_S { power2(n_) },
               m_O { hasO(m_) ? sum_of_combinations(n_,2) : 0 },
               m_A { hasA(m_) ? sum_of_combinations(n_,2) : 0 },
               m_N { hasN(m_) ? n_ : 0u },
               m_ON { hasON(m_) ? sum_of_combinations(n_,2) : 0 },
               m_AN { hasAN(m_) ? sum_of_combinations(n_,2) : 0 },
               m_I { (hasO(m_) ? sum_of_combinations(n_,2) : 0) +
                     (hasA(m_) ? sum_of_combinations(n_,2) : 0) +
                     (hasN(m_) ? n_ : 0) +
                     (hasAN(m_) ? sum_of_combinations(n_,2) : 0) +
                     (hasON(m_) ? sum_of_combinations(n_,2) : 0)
                   },
               m_T { n_ +
                     (hasO(m_) ? sum_of_combinations(n_,2) : 0) +
                     (hasA(m_) ? sum_of_combinations(n_,2) : 0) +
                     (hasN(m_) ? n_ : 0) +
                     (hasAN(m_) ? sum_of_combinations(n_,2) : 0) +
                     (hasON(m_) ? sum_of_combinations(n_,2) : 0)
                   },
               m_D { power2(power2(n_)) }
         {
            for (std::size_t i { 1 }; i <= m_n; ++i)
            {
               m_raw_independent_features.push_back(i);
            }
            for (std::size_t i { 2 }; i <= m_n; ++i)
            {
               combination_t c(raw_independent_features(),i);
               do
               {
                  m_raw_dependent_features.push_back(c());
               } while (c.next());
            }
            m_all_systems = generate_all_systems();
         }
         /*! Returns the number of independent features, same as F().
          */
         const auto& n() const
         {
            return m_n;
         }
         /*! Returns the number of independent features, same as n().
         */
         const auto& F() const
         {
            return m_F;
         }
         /*! Returns the number of systems of SPL.
         */
         const auto& S() const
         {
            return m_S;
         }
         /*! Returns the number of selected model.
         */
         const auto& M() const
         {
            return m_M;
         }
         /*! Returns the actual number of or-features.
         */
         const auto& O() const
         {
            return m_O;
         }
         /*! Returns the actual number of and-features.
         */
         const auto& A() const
         {
            return m_A;
         }
         /*! Returns the actual number of not-features.
         */
         const auto& N() const
         {
            return m_N;
         }
         /*! Returns actual number of or-not-features.
         */
         const auto& ON() const
         {
            return m_ON;
         }
         /*! Returns actual number of and-not-features.
         */
         const auto& AN() const
         {
            return m_AN;
         }
         /*! Returns the actual number of inherently dependent features.
         */
         const auto& DF() const
         {
            return m_I;
         }
         /*! Returns the actual total number of features.
         */
         const auto& T() const
         {
            return m_T;
         }
         /*! Returns the number of all possible set differences of SPL systems.
         */
         const auto& D() const
         {
            return m_D;
         }
         /*! Creates and returns a collection with all names of independent features.
             Takes raw feature IDs for single system.
          */
         feature_names_t generate_independent_features(const std::vector<feature_id_t>& f) const
         {
            std::vector<std::string> result;
            for (const auto& i : f)
            {
               result.push_back(independent_feature_name(i));
            }
            return result;
         }
         /*! Creates and returns a collection with all names of or-features.
             Takes raw feature IDs for single system.
          */
         feature_names_t generate_or_features(const std::vector<feature_id_t>& f) const
         {
            std::vector<std::string> result;
            if (hasO(M()))
            {
              for (auto i : f)
              {
                 for (const auto& j : raw_dependent_features())
                 if (auto t { or_feature_name(j) };
                     (contains(j,i) && !contains(result,t))
                    )
                 {
                    result.push_back(t);
                 }
              }
              sort(result.begin(),result.end());
            }
            return result;
         }
         /*! Creates and returns a collection with all names of and-features.
             Takes raw feature IDs for single system.
          */
         feature_names_t generate_and_features(const std::vector<feature_id_t>& f) const
         {
            std::vector<std::string> result;
            if (hasA(M()))
            {
               for (const auto& i : raw_dependent_features())
               {
                  bool valid { true };
                  for (auto j : i)
                  {
                     if (!contains(f,j))
                     {
                        valid = false;
                        break;
                     }
                  }
                  if (valid)
                  {
                     result.push_back(and_feature_name(i));
                  }
               }
               sort(result.begin(),result.end());
            }
            return result;
         }
         /*! Creates and returns a collection with all names of not-features.
             Takes raw feature IDs for single system.
          */
         feature_names_t generate_not_features(const std::vector<feature_id_t>& nf) const
         {
            std::vector<std::string> result;
            if (hasN(M()))
            {
               auto f { negate(nf,n()) };
               for (const auto& i : f)
               {
                  result.push_back(not_feature_name(i));
               }
            }
            return result;
         }
         /*! Creates and returns a collection with all names of or-not-features.
             Takes raw feature IDs for single system.          
          */
         feature_names_t generate_or_not_features(const std::vector<feature_id_t>& nf) const
         {
            std::vector<std::string> result;
            if (hasON(M()))
            {
               auto f { negate(nf,n()) };
               for (auto i : f)
               {
                  for (const auto& j : raw_dependent_features())
                  if (auto t { or_not_feature_name(j) };
                      (contains(j,i) && !contains(result,t))
                     )
                  {
                     result.push_back(t);
                  }
              }
              sort(result.begin(),result.end());
            }
            return result;
         }
         /*! Creates and returns a collection with all names of and-not-features.
             Takes raw feature IDs for single system.
          */
         feature_names_t generate_and_not_features(const std::vector<feature_id_t>& nf) const 
         {
            std::vector<std::string> result;
            if (hasAN(M()))
            {
               auto f { negate(nf,n()) };
               for (const auto& i : raw_dependent_features())
               {
                  bool valid { true };
                  for (auto j : i)
                  {
                     if (!contains(f,j))
                     {
                        valid = false;
                        break;
                     }
                  }
                  if (valid)
                  {
                     result.push_back(and_not_feature_name(i));
                  }
               }
               sort(result.begin(),result.end());
            }
            return result;
         }
         /*! Creates and returns all feature names that define a system. 
             Takes raw feature IDs for single system.
          */
         system_t generate_system(const std::vector<feature_id_t>& f)  const
         {
            std::vector<std::string> result;
            concat(result,generate_independent_features(f));
            concat(result,generate_or_features(f));
            concat(result,generate_and_features(f));
            concat(result,generate_not_features(f));
            concat(result,generate_or_not_features(f));
            concat(result,generate_and_not_features(f));
            sort(result.begin(),result.end());
            return result;
         }
         /*! Returns a collection of all systems of the SPL.
          */
         systems_t generate_all_systems() const 
         {
            std::vector<std::vector<std::string>> result;
            for (maxnat_t s { 0 }; s < S(); ++s)
            {
               result.push_back(generate_system(unsigned2vector(s)));
            }
            return result;
         }
         /*! Returns a collection with raw independent feature IDs. 
          */
         const std::vector<feature_id_t>& raw_independent_features() const
         {
            return m_raw_independent_features;
         }
         /*! Returns a collection with collection of raw feature IDs that
             represents dependent features (with the exception of not-features).
          */
         const std::vector<std::vector<feature_id_t>>& raw_dependent_features() const
         {
            return m_raw_dependent_features;
         }
         /*! Returns a collection wjth all systems.
          */
         const auto& all_systems() const
         {
            return m_all_systems;
         }
         /*! Prints header for results of feature location analysis.
             Takes output stream as parameter with std::cout as default value.
          */
         void print_header(std::ostream& os = std::cout)
         {
            os << "M" << M() << separator << "selected model\n"
               << T() << separator << "T" << separator << "actual total number of features\n"
               << F() << separator << "F" << separator << "number of independent features\n"
               << DF() << separator << "DF" << separator << "actual total number of inherently dependent features\n"
               << O() << separator << "O" << separator << "actual number of or-features\n"
               << A() << separator << "A" << separator << "actual number of and-features\n"
               << N() << separator << "N" << separator << "actual number of not-features\n"
               << ON() << separator << "ON" << separator << "actual number of or-not-features\n"
               << AN() << separator << "AN" << separator << "actual number of and-not-features\n"
               << S() << separator << "S" << separator << "number of systems of SPL\n"
               << D() << separator << "D" << separator << "number of all set differences of SPL systems\n";
         }

         /*! Prints all systems with the features that define them.
             Takes output stream as parameter with std::cout as default value.
          */
         void print_systems(std::ostream& os = std::cout)
         {
            for (maxnat_t i { 0 };const auto & system : all_systems())
            {
               os << system_name(++i) << separator;
               for (const auto& feature : system)
               {
                  os << feature << separator;
               }
               os << std::endl;
            }
         }
     private:
         const feature_id_t  m_n;
         const maxnat_t m_F;
         const feature_id_t m_M;

         const maxnat_t m_S,
                        m_O,
                        m_A,
                        m_N,
                        m_ON,
                        m_AN,
                        m_I,
                        m_T,
                        m_D;
         std::vector<feature_id_t> m_raw_independent_features;
         std::vector<std::vector<feature_id_t>> m_raw_dependent_features;
         std::vector<feature_names_t> m_all_systems;
   };

   /*! Class that performs feature location analysis
       based on feature isolation. In this approach,
       no IDs for set differences are created.
    */
   class feature_location_isolation_t: public feature_location_t
   {
      public:
         /*! Constructor that requires the number of independent features
             and the number of the model as parameters.
          */
         feature_location_isolation_t(feature_id_t n_,
                                      feature_id_t m_)
              :feature_location_t(n_,m_)
         { 
            m_independent_features = generate_independent_features();
            m_or_features = generate_or_features();
            m_and_features = generate_and_features();
            m_not_features = generate_not_features();
            m_or_not_features = generate_or_not_features();
            m_and_not_features = generate_and_not_features();
            m_all_features.reserve(m_all_features.size()
                                   + m_independent_features.size()
                                   + m_or_features.size()
                                   + m_and_features.size()
                                   + m_not_features.size()
                                   + m_or_not_features.size()
                                   + m_and_not_features.size()
                                 );
            concat(concat(concat(concat(concat(concat(m_all_features,
                   m_independent_features),m_or_features),m_and_features),
                   m_not_features),m_or_not_features),m_and_not_features
                  );
            std::sort(m_all_features.begin(),m_all_features.end());
            m_feature_isolations = generate_feature_isolations();
         }
         /*! Creates and returns a collection with all names of 
             independent features of all systems of the SPL.
          */
         feature_names_t generate_independent_features() const
         {
            return feature_location_t::generate_independent_features(raw_independent_features());
         }
         /*! Creates and returns a collection with all names of 
             or-features of all systems of the SPL.
          */
         feature_names_t generate_or_features() const
         {
            feature_names_t result;
            if (hasO(M()))
            {
               for (feature_id_t k { 2 }; k <= n(); ++k)
               {
                  combination_t combination { raw_independent_features(),k };
                  do 
                  {
                     result.push_back(or_feature_name(combination()));
                  } while (combination.next());
               }
            }
            return result;
         }
         /*! Creates and returns a collection with all names of 
             and-features of all systems of the SPL.
          */
         feature_names_t generate_and_features() const
         {
            feature_names_t result;
            if (hasA(M()))
            {
               for (feature_id_t k { 2 }; k <= n(); ++k)
               {
                  combination_t combination { raw_independent_features(),k };
                  do 
                  {
                     result.push_back(and_feature_name(combination()));
                  } while (combination.next());
               }
            }
            return result;
         }
         /*! Creates and returns a collection with all names of 
             not-features of all systems of the SPL.
          */
         feature_names_t generate_not_features() const
         {
            feature_names_t result;
            if (hasN(M()))
            {
               for (const auto& i : raw_independent_features())            {
                  result.push_back(not_feature_name(i));
               }
            }
            return result;
         }
         /*! Creates and returns a collection with all names of 
             or-not-features of all systems of the SPL.
          */
         feature_names_t generate_or_not_features() const
         {
            feature_names_t result;
            if (hasON(M()))
            {
               for (feature_id_t k { 2 }; k <= N(); ++k)
               {
                  combination_t combination { raw_independent_features(),k };
                  do 
                  {
                     result.push_back(or_not_feature_name(combination()));
                  } while (combination.next());
               }
            }
            return result;
         }
         /*! Creates and returns a collection with all names of 
             and-not-features of all systems of the SPL.
          */
         feature_names_t generate_and_not_features() const
         {
            feature_names_t result;
            if (hasAN(M()))
            {
               for (feature_id_t k { 2 }; k <= N(); ++k)
               {
                  combination_t combination { raw_independent_features(),k };
                  do 
                  {
                     result.push_back(and_not_feature_name(combination()));
                  } while (combination.next());
               }
            }
            return result;
         }
         /*! Returns collection with all independent features.
          */
         const feature_names_t& independent_features() const
         {
            return m_independent_features;
         }
         /*! Returns collection with all or-features.
          */
         const std::vector<std::string>& or_features() const
         {
            return m_or_features;
         }
         /*! Returns collection with all and-features.
          */
         const std::vector<std::string>& and_features() const
         {
            return m_and_features;
         }
         /*! Returns collection with all not-features.
          */
         const std::vector<std::string>& not_features() const
         {
            return m_not_features;
         }
         /*! Returns collection with all or-not-features.
          */
         const std::vector<std::string>& or_not_features() const
         {
            return m_or_not_features;
         }
         /*! Returns collection with all and-not-features.
          */
         const std::vector<std::string>& and_not_features() const
         {
            return m_and_not_features;
         }
         /*! Returns collection with all features.
          */
         const std::vector<std::string>& all_features() const
         {
            return m_all_features;
         }
         /*! Returns collection with all feature isolations.
          */
         const features_isolation_t& feature_isolations() const
         {
            return m_feature_isolations;
         }
         /*! Creates and returns collection with all feature isolations.
          */
         features_isolation_t generate_feature_isolations() const
         {
            features_isolation_t result;
            for (const auto& f : all_features())
            {
               systems_difference_t difference;
               maxnat_t system_no { 0 };
               for (const auto& s: all_systems())
               {
                  ++system_no;
                  if (contains(s,f))
                  {
                     difference.intersections.insert(system_name(system_no));
                  }
                  else
                  {
                    difference.unions.insert(system_name(system_no));
                  }
               }
               result.insert(std::make_pair(f,difference));
            }
            return result;
         }   
         /*! Prints intersection part, that is the left operand, of set difference.
             Takes output stream as parameter with std::cout as default value.
          */
         void print_intersections(const std::set<std::string>& s,std::ostream& os = std::cout) const
         {
           std::size_t counter { },
                       size { s.size() };
            os << opening_parenthesis << set_separator;
            for (const auto& e : s)
            {
               os << e;
               if (++counter < size)
               {
                  os << set_separator << set_intersection << set_separator;
               }
            }
            os << set_separator << closing_parenthesis;
         }
         /*! Prints union part, that is the right operand, of set difference.
             Takes output stream as parameter with std::cout as default value.
          */
         void print_unions(const std::set<std::string>& s,std::ostream& os = std::cout) const
         {
            std::size_t counter { },
                        size { s.size() };
            os << opening_parenthesis << set_separator;
            for (const auto& e : s)
            {
               os << e;
               if (++counter < size)
               {
                  os << set_separator << set_union << set_separator;
               }
            }
            os << set_separator << closing_parenthesis;
         }
         /*! Prints results of feature isolation.
             Takes output stream as parameter with std::cout as default value.
          */
         void print_results(std::ostream& os = std::cout) const
         {
            for (const auto& e : feature_isolations())
            {
               os << e.first << separator;
               print_intersections(e.second.intersections,os);
               os << set_separator << set_difference << set_separator;
               print_unions(e.second.unions,os);
               os << std::endl;
            }
         }
      private:
         std::vector<std::string> m_independent_features,
                                  m_or_features,
                                  m_and_features,
                                  m_not_features,
                                  m_or_not_features,
                                  m_and_not_features,
                                  m_all_features;
         features_isolation_t     m_feature_isolations;
   };

   /*! Class that performs feature location analysis
       by calculating all set differences. In this approach,
       IDs for set differences are created.
    */
  class feature_location_differences_t: public feature_location_isolation_t
   {
      public:
         /*! Constructor that requires the number of idependent features
             and the number of the model as parameters.
          */
         feature_location_differences_t(feature_id_t n_,
                                        feature_id_t m_)
              :feature_location_isolation_t(n_,m_)
         { 
            m_non_empty_differences = generate_non_empty_differences();
         }
         /*! Takes a string containing all defining features of a system
             and returs a set of all features that define the system.
          */
         std::set<std::string> system2features(std::string s) const
         {
            if (s.length() < system.length() + 1 || s.find(system) != 0)
            {
               throw std::logic_error(s + " is not a system identifier.");
            }
            std::set<std::string> result;
            std::size_t pos { stoull(s.erase(0,system.size())) };
            for (const auto& f : all_systems().at(pos - 1))
            {
               result.insert(f);
            }
            return result;
         }
         /*! Calculates and returns a set difference expression
             for a single system.
             Takes total number of systems as parameter.
          */
         systems_difference_t generate_difference(maxnat_t s) const
         {
            systems_difference_t result;
            for (maxnat_t position { 0 }; position < S(); ++position)
            {
               if (s & 1ull << position)
               {
                  result.intersections.insert(system_name(position + 1));
               }
               else
               {
                  result.unions.insert(system_name(position + 1));
               }
            }
            return result;
         }
         /*! Evaluates the intersection part of a set difference and
             returns the result.
             Takes a set difference as parameter.
          */
         std::set<std::string> evaluate_intersections(const systems_difference_t& diff) const
         {
            std::set<std::string> result;
            if (diff.intersections.size() >= 1)
            {
               auto first { diff.intersections.cbegin() };
               result = system2features(*first);
               while (++first != diff.intersections.end())
               {
                  auto left { result };
                  auto right { system2features(*first) };
                  result.clear();
                  std::set_intersection(left.begin(),left.end(),
                                        right.begin(),right.end(),
                                        std::inserter(result,result.begin()));
                  if (result.size() == 0)
                  {
                     break;
                  }
               }
            }
            return result;
         }
         /*! Evaluates the union part of a set difference and
             returns the result.
             Takes a set difference as parameter.
          */
         std::set<std::string> evaluate_unions(const systems_difference_t& diff) const
         {
            std::set<std::string> result;
            if (diff.unions.size() >= 1)
            {
               auto first { diff.unions.cbegin() };
               result = system2features(*first);
               while (++first != diff.unions.end())
               {
                  auto left { result };
                  auto right { system2features(*first) };
                  result.clear();
                  std::set_union(left.begin(),left.end(),
                                 right.begin(),right.end(),
                                 std::inserter(result,result.begin()));
               }
            }
            return result;
         }
         /*! Evaluates a set difference and returns the result.
             Takes the set difference as parameter.
          */
         std::set<std::string> evaluate_difference(const systems_difference_t& diff) const
         {
            auto intersections { evaluate_intersections(diff) };
            auto unions { evaluate_unions(diff) };
            std::set<std::string> result;
            std::set_difference(intersections.begin(),intersections.end(),
                                unions.begin(),unions.end(),
                                std::inserter(result,result.begin()));

            return result;
         }
         differences_t generate_non_empty_differences() const
         {
            differences_t result { };
            for (maxnat_t e { 1 }; e < D(); ++e)
            {
               const auto& diff { generate_difference(e) };
               const auto& res { evaluate_difference(diff) };
               if (res.size() > 0)
               {
                  if (res.size() != 1)
                  {
                     throw std::length_error("Set size = " + std::to_string(res.size()) +  ", must be 1!");
                  }
                  result.push_back(system_feature_difference_t { e,*res.begin(),diff });
               }
            }
            return result;
         }
         /*! Returns collection with non-empty set differences.
          */
         differences_t non_empty_differences() const
         {
            return m_non_empty_differences;
         }
         /*! Prints results of evaluating all set differences.
             Takes output stream as parameter with std::cout as default value.
          */
         void print_results(std::ostream& os = std::cout) const
         {
            for (const auto& e : non_empty_differences())
            {
               os << difference_name(e.difference_id) << separator
                  << e.feature << separator;
               print_intersections(e.difference.intersections,os);
               os << set_separator << set_difference << set_separator;
               print_unions(e.difference.unions,os);
               os << std::endl;
            }
         }
      private:
         differences_t m_non_empty_differences;
   };

   /*! Class that performs feature location analysis
       by calculating only valid set differences. In this approach,
       only IDs for valid set differences are created.
    */
   class feature_location_calculation_t: public feature_location_t
   {
      public:
         /*! Constructor that requires the number of idependent features
             and the number of the model as parameters.
          */
         feature_location_calculation_t(feature_id_t n_,
                                        feature_id_t m_)
              :feature_location_t(n_,m_),
               m_systems_bitmask { initialize_bitmask() }
         { 
            m_independent_features = calculate_independent_features();
            m_or_features = calculate_or_features();
            m_and_features = calculate_and_features();
            m_not_features = calculate_not_features();
            m_or_not_features = calculate_or_not_features();
            m_and_not_features = calculate_and_not_features();
            concat(concat(concat(concat(concat(concat(m_all_features,
                   m_independent_features),m_or_features),m_and_features),
                   m_not_features),m_or_not_features),m_and_not_features
                  );
            m_differences = calculate_differences();
         }
         /*! Calculates and returns bit mask that masks
             non-existant systems.
          */
         maxnat_t initialize_bitmask() const
         {
            maxnat_t bitmask {};
            for (maxnat_t s {}; s < S(); ++s)
            {
              bitmask |= (1llu << s);
            }
            return ~bitmask; 
         }
         /*! Returns bit mask that masks non-existant systems.
          */
         const maxnat_t& systems_bitmask() const
         {
            return m_systems_bitmask;
         }
         /*! Calculates and returns set difference.
             Takes system ID and system name as parameters.
          */
         system_feature_difference_t calculate_difference(maxnat_t index,const std::string& name) const
         {
            system_feature_difference_t result { index,name };
            for (maxnat_t s { 0llu }; s < S(); ++s)
            {
               if (index & (1llu << s))
               {
                  result.difference.intersections.insert(system_name(s + 1));
               }
               else
               {
                  result.difference.unions.insert(system_name(s + 1));
               }
            }
            return result;
         }
         /*! Evaluates and returns collection with all valid system differences.
          */
         differences_t calculate_differences() const
         {
            differences_t result;
            for (const auto& [name,value]  : all_features())
            {
               result.push_back(calculate_difference(value,name));
            }
            sort(result.begin(),result.end(),[] (auto a,auto b) -> bool { return a.difference_id < b.difference_id; });
            return result;
         }
         /*! Returns collection with valid system differences.
          */
         differences_t differences() const
         {
            return m_differences;
         }
         /*! Calculates and returns collection of independent features.
          */
         feature_expression_t calculate_independent_features() const
         {
            feature_expression_t result;
            for (maxnat_t f { 0 }; f < n(); ++f)
            {
               maxnat_t value { 0 };
               maxnat_t bit { 1 };
               maxnat_t counter { power2(f) };
               for (maxnat_t s { 0 }; s < power2(n()) - 1; ++s)
               {
                  value |= bit;
                  --counter;
                  value <<= 1llu;
                  if (counter == 0llu)
                  {
                     bit = (bit == 1llu ? 0llu : 1llu);
                     counter = power2(f);
                  }
               }
               result.push_back(make_pair(independent_feature_name(f + 1),value));
            }
            return result;
         }
         /*! Calculates and returns collection of or-features.
          */
         feature_expression_t calculate_or_features() const
         {
            feature_expression_t result;
            if (hasO(M()))
            {
               for (feature_id_t k { 2 }; k <= n(); ++k)
               {
                  combination_t combination { raw_independent_features(),k };
                  do 
                  {
                     result.push_back( { or_feature_name(combination()), or_feature_value(combination(),independent_features()) } );
                  } while (combination.next());
               }
            }
            return result;
         }
         /*! Calculates and returns collection of and-features.
          */
         feature_expression_t calculate_and_features() const
         {
            feature_expression_t result;
            if (hasA(M()))
            {
               for (feature_id_t k { 2 }; k <= n(); ++k)
               {
                  combination_t combination { raw_independent_features(),k };
                  do 
                  {
                     result.push_back( { and_feature_name(combination()), and_feature_value(combination(),independent_features()) } );
                  } while (combination.next());
               }
            }
            return result;
         }
         /*! Calculates and returns collection of not-features.
          */
         feature_expression_t calculate_not_features() const
         {
            feature_expression_t result;
            if (hasN(M()))
            {
               for (const auto& f : independent_features())
               {
                  // Possible source of errors: The following line does not call
                  // not_feature_name()! Instead it uses feature_not + f.first!
                  result.push_back(make_pair(feature_not + f.first,~(f.second ^ systems_bitmask())));
               }
            }
            return result;
         }
         /*! Calculates and returns collection of or-not-features.
          */
         feature_expression_t calculate_or_not_features() const
         {
            feature_expression_t result;
            if (hasON(M()))
            {
               for (feature_id_t k { 2 }; k <= n(); ++k)
               {
                  combination_t combination { raw_independent_features(),k };
                  do 
                  {
                     result.push_back( { or_not_feature_name(combination()), or_not_feature_value(combination(),not_features()) } );
                  } while (combination.next());
               }
            }
            return result;
         }
         /*! Calculates and returns collection of and-not-features.
          */
         feature_expression_t calculate_and_not_features() const
         {
            feature_expression_t result;
            if (hasAN(M()))
            {
               for (feature_id_t k { 2 }; k <= n(); ++k)
               {
                  combination_t combination { raw_independent_features(),k };
                  do 
                  {
                     result.push_back( { and_not_feature_name(combination()), and_not_feature_value(combination(),not_features(),systems_bitmask()) } );
                  } while (combination.next());
               }
            }
            return result;
         }
         /*! Returns collection with all independent feature names plus difference ID plus set difference.
          */
         feature_expression_t independent_features() const
         {
            return m_independent_features;
         }
         /*! Returns collection with all or-feature names plus difference ID plus set difference.
          */
         feature_expression_t or_features() const
         {
            return m_or_features;
         }
         /*! Returns collection with all and-feature names plus difference ID plus set difference.
          */
         feature_expression_t and_features() const
         {
            return m_and_features;
         }
         /*! Returns collection with all not-feature names plus difference ID plus set difference.
          */
         feature_expression_t not_features() const
         {
            return m_not_features;
         }
         /*! Returns collection with all or-not-feature names plus difference ID plus set difference.
          */
         feature_expression_t or_not_features() const
         {
            return m_or_not_features;
         }
         /*! Returns collection with all and-not-feature names plus difference ID plus set difference.
          */
         feature_expression_t and_not_features() const
         {
            return m_and_not_features;
         }
         /*! Returns collection with all feature names plus difference ID plus set difference.
          */
         feature_expression_t all_features() const
         {
            return m_all_features;
         }
         /*! Prints intersection part, that is the left operand, of set difference.
             Takes output stream as parameter with std::cout as default value.
          */
         void print_intersections(const std::set<std::string>& s,std::ostream& os = std::cout) const
         {
           std::size_t counter { },
                       size { s.size() };
            os << opening_parenthesis << set_separator;
            for (const auto& e : s)
            {
               os << e;
               if (++counter < size)
               {
                  os << set_separator << set_intersection << set_separator;
               }
            }
            os << set_separator << closing_parenthesis;
         }
         /*! Prints union part, that is the right operand, of set difference.
             Takes output stream as parameter with std::cout as default value.
          */
         void print_unions(const std::set<std::string>& s,std::ostream& os = std::cout) const
         {
            std::size_t counter { },
                        size { s.size() };
            os << opening_parenthesis << set_separator;
            for (const auto& e : s)
            {
               os << e;
               if (++counter < size)
               {
                  os << set_separator << set_union << set_separator;
               }
            }
            os << set_separator << closing_parenthesis;
         }
         /*! Prints results of feature isolation.
             Takes output stream as parameter with std::cout as default value.
          */
         void print_results(std::ostream& os = std::cout) const
         {
            for (const auto& e : differences())
            {
               os << difference_name(e.difference_id) << separator
                  << e.feature << separator;
               print_intersections(e.difference.intersections,os);
               os << set_separator << set_difference << set_separator;
               print_unions(e.difference.unions,os);
               os << std::endl;
            }
         }
      private:
         const maxnat_t m_systems_bitmask;
         differences_t m_differences;
         feature_expression_t m_independent_features,
                              m_or_features,
                              m_and_features,
                              m_not_features,
                              m_or_not_features,
                              m_and_not_features,
                              m_all_features;
   };
}

#endif // FEATURES_H
