// Distributed under the MIT License.
// See LICENSE.txt for details.

#include "Evolution/Systems/RelativisticEuler/Valencia/BoundaryConditions/BoundaryCondition.hpp"

#include <pup.h>

#include "Domain/BoundaryConditions/BoundaryCondition.hpp"
#include "Utilities/GenerateInstantiations.hpp"

namespace RelativisticEuler::Valencia::BoundaryConditions {
template <size_t Dim>
BoundaryCondition<Dim>::BoundaryCondition(CkMigrateMessage* const msg)
    : domain::BoundaryConditions::BoundaryCondition(msg) {}

template <size_t Dim>
void BoundaryCondition<Dim>::pup(PUP::er& p) {
  domain::BoundaryConditions::BoundaryCondition::pup(p);
}

#define DIM(data) BOOST_PP_TUPLE_ELEM(0, data)

#define INSTANTIATION(r, data) template class BoundaryCondition<DIM(data)>;

GENERATE_INSTANTIATIONS(INSTANTIATION, (1, 2, 3))

#undef INSTANTIATION
#undef DIM
}  // namespace RelativisticEuler::Valencia::BoundaryConditions
