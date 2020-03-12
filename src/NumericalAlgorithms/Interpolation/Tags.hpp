// Distributed under the MIT License.
// See LICENSE.txt for details.

#pragma once

#include <cstddef>
#include <deque>
#include <string>
#include <type_traits>
#include <unordered_map>
#include <unordered_set>

#include "DataStructures/DataBox/PrefixHelpers.hpp"
#include "DataStructures/DataBox/Tag.hpp"
#include "DataStructures/Variables.hpp"
#include "Domain/Mesh.hpp"
#include "NumericalAlgorithms/Interpolation/InterpolatedVars.hpp"
#include "Options/Options.hpp"
#include "Utilities/TaggedTuple.hpp"

/// \cond
template <size_t VolumeDim>
class ElementId;
/// \endcond

namespace intrp {


namespace OptionTags {
/*!
 * \ingroup OptionGroupsGroup
 * \brief Groups option tags for InterpolationTargets.
 */
struct InterpolationTargets {
  static constexpr OptionString help{"Options for interpolation targets"};
};
}  // namespace OptionTags

/// Tags for items held in the `DataBox` of `InterpolationTarget` or
/// `Interpolator`.
namespace Tags {

/// Keeps track of which points have been filled with interpolated data.
struct IndicesOfFilledInterpPoints : db::SimpleTag {
  using type = std::unordered_set<size_t>;
};

/// Keeps track of points that cannot be filled with interpolated data.
///
/// The InterpolationTarget can decide what to do with these points.
/// In most cases the correct action is to throw an error, but in other
/// cases one might wish to fill these points with a default value or
/// take some other action.
struct IndicesOfInvalidInterpPoints : db::SimpleTag {
  using type = std::unordered_set<size_t>;
};

/// `temporal_id`s on which to interpolate.
template <typename Metavariables>
struct TemporalIds : db::SimpleTag {
  using type = std::deque<typename Metavariables::temporal_id::type>;
};

/// `temporal_id`s that we have already interpolated onto.
///  This is used to prevent problems with multiple late calls to
///  AddTemporalIdsToInterpolationTarget.
template <typename Metavariables>
struct CompletedTemporalIds : db::SimpleTag {
  using type = std::deque<typename Metavariables::temporal_id::type>;
};

/// Volume variables at all `temporal_id`s for all local `Element`s.
template <typename Metavariables>
struct VolumeVarsInfo : db::SimpleTag {
  struct Info {
    Mesh<Metavariables::volume_dim> mesh;
    Variables<typename Metavariables::interpolator_source_vars> vars;

    void pup(PUP::er& p) noexcept {  // NOLINT
      p | mesh;
      p | vars;
    }
  };
  using type = std::unordered_map<
      typename Metavariables::temporal_id::type,
      std::unordered_map<ElementId<Metavariables::volume_dim>, Info>>;
};

namespace holders_detail {
template <typename InterpolationTargetTag, typename Metavariables>
using WrappedHolderTag = Vars::HolderTag<InterpolationTargetTag, Metavariables>;
}  // namespace holders_detail

/// `TaggedTuple` containing all local `Vars::Holder`s for
/// all `InterpolationTarget`s.
///
/// A particular `Vars::Holder` can be retrieved from this
/// `TaggedTuple` via a `Vars::HolderTag`.  An `Interpolator` uses the
/// object in `InterpolatedVarsHolders` to iterate over all of the
/// `InterpolationTarget`s.
template <typename Metavariables>
struct InterpolatedVarsHolders : db::SimpleTag {
  using type = tuples::tagged_tuple_from_typelist<db::wrap_tags_in<
      holders_detail::WrappedHolderTag,
      typename Metavariables::interpolation_target_tags, Metavariables>>;
};

/// Number of local `Element`s.
struct NumberOfElements : db::SimpleTag {
  using type = size_t;
};

}  // namespace Tags
}  // namespace intrp
