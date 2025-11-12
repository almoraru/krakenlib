/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: almoraru <almoraru@student.fr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/08 11:25:39 by almoraru          #+#    #+#             */
/*      Updated: 2025/11/11 22:38:54 by espadara                              */
/*                                                                            */
/* ************************************************************************** */

#include "sea_malloc.h"

bool error_malloc(const int err, const char *str)
{
  static const char *msg[3] = {
    "  -> in _",
    "Invalid block: ",
    "mmap MAP_FAILED error: "};
  sea_printf("%s%s\n", msg[err], str);
  return (false);
}
