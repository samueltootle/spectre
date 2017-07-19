// Distributed under the MIT License.
// See LICENSE.txt for details.

#include <catch.hpp>

#include "DataStructures/Tensor/EagerMath/Magnitude.hpp"

TEST_CASE("Unit.DataStructures.Tensor.EuclideanMagnitude",
          "[DataStructures][Unit]") {
  const size_t npts = 2;
  const DataVector one(npts, 1.0);
  const DataVector two(npts, 2.0);
  const DataVector minus_three(npts, -3.0);
  const DataVector four(npts, 4.0);
  const DataVector minus_five(npts, -5.0);
  const DataVector twelve(npts, 12.0);

  const tnsr::i<DataVector, 1, Frame::Grid> one_d_covector{{{two}}};
  const DataVector magnitude_one_d_covector = magnitude(one_d_covector);
  for (size_t s = 0; s < npts; ++s) {
    CHECK(magnitude_one_d_covector[s] == 2.0);
  }

  const tnsr::i<DataVector, 1, Frame::Grid> negative_one_d_covector{
      {{minus_three}}};
  const DataVector magnitude_negative_one_d_covector =
      magnitude(negative_one_d_covector);
  for (size_t s = 0; s < npts; ++s) {
    CHECK(magnitude_negative_one_d_covector[s] == 3.0);
  }
  const tnsr::A<DataVector, 1, Frame::Grid> one_d_vector{{{minus_three, four}}};
  const DataVector magnitude_one_d_vector = magnitude(one_d_vector);
  for (size_t s = 0; s < npts; ++s) {
    CHECK(magnitude_one_d_vector[s] == 5.0);
  }

  const tnsr::I<DataVector, 2, Frame::Grid> two_d_vector{
      {{minus_five, twelve}}};
  const DataVector magnitude_two_d_vector = magnitude(two_d_vector);
  for (size_t s = 0; s < npts; ++s) {
    CHECK(magnitude_two_d_vector[s] == 13.0);
  }

  const tnsr::i<DataVector, 3, Frame::Grid> three_d_covector{
      {{minus_three, twelve, four}}};
  const DataVector magnitude_three_d_covector = magnitude(three_d_covector);
  for (size_t s = 0; s < npts; ++s) {
    CHECK(magnitude_three_d_covector[s] == 13.0);
  }

  // 5D example
  const tnsr::a<DataVector, 4, Frame::Grid> five_d_covector{
      {{two, twelve, four, one, two}}};
  const DataVector magnitude_five_d_covector = magnitude(five_d_covector);
  for (size_t s = 0; s < npts; ++s) {
    CHECK(magnitude_five_d_covector[s] == 13.0);
  }
}

TEST_CASE("Unit.DataStructures.Tensor.Magnitude", "[DataStructures][Unit]") {
  const size_t npts = 2;
  const DataVector one(npts, 1.0);
  const DataVector two(npts, 2.0);
  const DataVector minus_three(npts, -3.0);
  const DataVector four(npts, 4.0);
  const DataVector minus_five(npts, -5.0);
  const DataVector twelve(npts, 12.0);
  const DataVector thirteen(npts, 13.0);

  const tnsr::i<DataVector, 1, Frame::Grid> one_d_covector{{{two}}};
  const tnsr::II<DataVector, 1, Frame::Grid> inv_h = [&four]() {
    tnsr::II<DataVector, 1, Frame::Grid> tensor;
    tensor.template get<0, 0>() = four;
    return tensor;
  }();

  const DataVector magnitude_a = magnitude(one_d_covector, inv_h);
  for (size_t s = 0; s < npts; ++s) {
    CHECK(magnitude_a[s] == 4.0);
  }
  const tnsr::i<DataVector, 3, Frame::Grid> three_d_covector{
      {{minus_three, twelve, four}}};
  const tnsr::II<DataVector, 3, Frame::Grid> inv_g =
      [&two, &minus_three, &four, &minus_five, &twelve, &thirteen]() {
        tnsr::II<DataVector, 3, Frame::Grid> tensor;
        tensor.get<0, 0>() = two;
        tensor.get<0, 1>() = minus_three;
        tensor.get<0, 2>() = four;
        tensor.get<1, 1>() = minus_five;
        tensor.get<1, 2>() = twelve;
        tensor.get<2, 2>() = thirteen;
        return tensor;
      }();
  const DataVector magnitude_three_d_covector =
      magnitude(three_d_covector, inv_g);
  for (size_t s = 0; s < npts; ++s) {
    CHECK(magnitude_three_d_covector[s] == sqrt(778.0));
  }
}
