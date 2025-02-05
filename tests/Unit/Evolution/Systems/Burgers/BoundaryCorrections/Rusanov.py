# Distributed under the MIT License.
# See LICENSE.txt for details.

import numpy as np


def dg_package_data_u(
    u, flux_u, normal_covector, mesh_velocity, normal_dot_mesh_velocity
):
    return u


def dg_package_data_normal_dot_flux(
    u, flux_u, normal_covector, mesh_velocity, normal_dot_mesh_velocity
):
    return np.einsum("i,i", normal_covector, flux_u)


def dg_package_data_abs_char_speed(
    u, flux_u, normal_covector, mesh_velocity, normal_dot_mesh_velocity
):
    if normal_dot_mesh_velocity is None:
        return np.abs(u)
    else:
        return np.abs(u - normal_dot_mesh_velocity)


def dg_boundary_terms_u(
    interior_u,
    interior_normal_dot_flux_u,
    interior_abs_char_speed,
    exterior_u,
    exterior_normal_dot_flux_u,
    exterior_abs_char_speed,
    use_strong_form,
):
    if use_strong_form:
        return -0.5 * (
            interior_normal_dot_flux_u + exterior_normal_dot_flux_u
        ) - 0.5 * np.maximum(
            interior_abs_char_speed, exterior_abs_char_speed
        ) * (
            exterior_u - interior_u
        )
    else:
        return 0.5 * (
            interior_normal_dot_flux_u - exterior_normal_dot_flux_u
        ) - 0.5 * np.maximum(
            interior_abs_char_speed, exterior_abs_char_speed
        ) * (
            exterior_u - interior_u
        )
