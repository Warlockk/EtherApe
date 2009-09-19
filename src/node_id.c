/* EtherApe
 * Copyright (C) 2001-2009 Juan Toledo, Riccardo Ghetta
 * $Id$
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include "globals.h"
#include "node_id.h"

/***************************************************************************
 *
 * node_id_t implementation
 *
 **************************************************************************/

/* Comparison function used to order the (GTree *) nodes
 * and canvas_nodes heard on the network */
gint
node_id_compare (const node_id_t * na, const node_id_t * nb)
{
  const guint8 *ga = NULL;
  const guint8 *gb = NULL;
  int i = 0;

  g_assert (na != NULL);
  g_assert (nb != NULL);
  if (na->node_type < nb->node_type)
    return -1;
  else if (na->node_type > nb->node_type)
    return 1;

  /* same node type, compare */
  switch (na->node_type)
    {
    case ETHERNET:
      ga = na->addr.eth;
      gb = nb->addr.eth;
      i = sizeof(na->addr.eth);
      break;
    case FDDI:
      ga = na->addr.fddi;
      gb = nb->addr.fddi;
      i = sizeof(na->addr.fddi);
      break;
    case IEEE802:
      ga = na->addr.i802;
      gb = nb->addr.i802;
      i = sizeof(na->addr.i802);
      break;
    case IP:
      ga = na->addr.ip4;
      gb = nb->addr.ip4;
      i = sizeof(na->addr.ip4);
      break;
    case TCP:
      ga = na->addr.tcp4.host;
      gb = nb->addr.tcp4.host;
      i = sizeof(na->addr.tcp4.host)+sizeof(na->addr.tcp4.port); /* full struct size */
      break;
    default:
      g_error (_("Unsopported ape mode in node_id_compare"));
    }

  while (i)
    {
      if (ga[i] < gb[i])
	{
	  return -1;
	}
      else if (ga[i] > gb[i])
	return 1;
      --i;
    }

  return 0;
}				/* node_id_compare */

gchar *node_id_dump(const node_id_t *id)
{
  gchar *msg;

  switch (id->node_type)
    {
    case ETHERNET:
      msg = g_strdup_printf("ETH: %s", ether_to_str(id->addr.eth));
      break;
    case FDDI:
      msg = g_strdup_printf("FDDI: %s", ether_to_str(id->addr.fddi));
      break;
    case IEEE802:
      msg = g_strdup_printf("IEEE802: %s", ether_to_str(id->addr.fddi));
      break;
    case IP:
      msg = g_strdup_printf("IP: %s", ip_to_str (id->addr.ip4));
      break;
    case TCP:
      msg = g_strdup_printf("TCP/UDP: %s:%d", ip_to_str (id->addr.tcp4.host), 
                            *(guint16 *) (id->addr.tcp4.port));
      break;
    default:
      msg = g_strdup_printf("node_id_type unknown");
      break;
    }

  return msg;
}

/***************************************************************************
 *
 * name_t implementation
 *
 **************************************************************************/
name_t * node_name_create(const node_id_t *node_id)
{
  name_t *name;
  
  g_assert(node_id);
  name = g_malloc (sizeof (name_t));
  name->node_id = *node_id;
  name->n_packets = 0;
  name->accumulated = 0;
  name->numeric_name = NULL;
  name->name = NULL;
  return name;
}

void node_name_delete(name_t * name)
{
  if (name)
  {
    g_string_free (name->name, TRUE);
    g_string_free (name->numeric_name, TRUE);
    g_free (name);
  }
}

void node_name_assign(name_t * name, const gchar *nm, const gchar *num_nm, 
                 gboolean slv, gdouble sz)
{
  g_assert(name);
  if (!name->numeric_name)
    name->numeric_name = g_string_new (num_nm);
  else
    g_string_assign (name->numeric_name, num_nm);

  if (!name->name)
    name->name = g_string_new (nm);
  else
    g_string_assign (name->name, nm);
  
  name->solved = slv;
  name->n_packets++;
  name->accumulated += sz;
}

gchar *node_name_dump(const name_t *name)
{
  gchar *msg;
  gchar *nid;
  if (!name)
    return g_strdup("node_name_t NULL");
  
  nid = node_id_dump(&name->node_id);
  msg = g_strdup_printf("node id: %s, name: %s, numeric_name: %s, solved: %d, "
                        "accumulated %f, packets %f",
                        nid, name->name->str, name->numeric_name->str,
                        name->solved, name->accumulated, name->n_packets);
  g_free(nid);
  return msg;
}

/* compares by node id */
gint 
node_name_id_compare(const name_t *a, const name_t *b)
{
  g_assert (a != NULL);
  g_assert (b != NULL);
  return node_id_compare(&a->node_id, &b->node_id);
}

/* Comparison function to sort protocols by their accumulated traffic */
gint
node_name_freq_compare (gconstpointer a, gconstpointer b)
{
  const name_t *name_a, *name_b;

  g_assert (a != NULL);
  g_assert (b != NULL);

  name_a = (const name_t *) a;
  name_b = (const name_t *) b;

  if (name_a->accumulated > name_b->accumulated)
    return -1;
  if (name_a->accumulated < name_b->accumulated)
    return 1;
  return 0;
}
