/* Etherape
 * Copyright (C) 2000 Juan Toledo
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

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <gnome.h>

#include "interface.h"
#include "support.h"
#include "math.h"
#include "eth_resolv.h"

#include "globals.h"
#include "diagram.h"
#include "callbacks.h"


struct popup_data
  {
    GtkWidget *node_popup;
    canvas_node_t *canvas_node;
  };


/* Local functions definitions */

gchar *
get_prot_color (gchar * name)
{
  /* TODO This is all hardwired now. This should read preferences
   * and whatnot */

  if (!strcmp (name, "IP"))
    return "red";
  if (!strcmp (name, "ARP"))
    return "blue";
  if (!strcmp (name, "ATALK"))
    return "yellow";
  if (!strcmp (name, "IPX"))
    return "white";
  if (!strcmp (name, "VINES"))
    return "orange";
  if (!strcmp (name, "X25L3"))
    return "green";
  if (!strcmp (name, "IPv6"))
    return "white";
  if (!strcmp (name, "VLAN"))
    return "cyan";
  if (!strcmp (name, "SNMP"))
    return "orange";
  if (!strcmp (name, "802.3"))
    return "brown";
  if (!strcmp (name, "802.2"))
    return "purple";

  /* UNKNOWN */
  return "tan";
}				/* get_prot_color */

gdouble
get_node_size (gdouble average)
{
  gdouble result = 0.0;
  switch (size_mode)
    {
    case LINEAR:
      result = average + 1;
      break;
    case LOG:
      result = log (average + 1);
      break;
    case SQRT:
      result = sqrt (average + 1);
      break;
    }
  return (double) (5 + node_radius_multiplier * result);
}

gdouble
get_link_size (gdouble average)
{
  gdouble result = 0.0;
  switch (size_mode)
    {
    case LINEAR:
      result = average + 1;
      break;
    case LOG:
      result = log (average + 1);
      break;
    case SQRT:
      result = sqrt (average + 1);
      break;
    }
  return (double) (1 + link_width_multiplier * result);
}

static gint
link_item_event (GnomeCanvasItem * item, GdkEvent * event,
		 canvas_link_t * canvas_link)
{
  static GnomeAppBar *appbar;
  gchar *str;

  if (!appbar)
    appbar = GNOME_APPBAR (lookup_widget (GTK_WIDGET (app1), "appbar1"));

  switch (event->type)
    {

    case GDK_ENTER_NOTIFY:
      str = g_strdup_printf (_ ("Link main protocol: %s"),
			     canvas_link->link->main_prot);
      gnome_appbar_push (appbar, str);
      g_free (str);
      break;
    case GDK_LEAVE_NOTIFY:
      gnome_appbar_pop (appbar);
      break;
    default:
      break;
    }

  return FALSE;
}

guint
popup_to (struct popup_data * pd)
{

  GtkLabel *label;
  gchar *str;

  pd->node_popup = create_node_popup ();
  label = (GtkLabel *) lookup_widget (GTK_WIDGET (pd->node_popup), "name");

  if (mode == ETHERNET && pd->canvas_node->node->ip_address)
    {
      str = g_strdup_printf ("%s (%s, %s)",
			     pd->canvas_node->node->name->str,
			     pd->canvas_node->node->numeric_ip->str,
			     pd->canvas_node->node->numeric_name->str);
      gtk_label_set_text (label, str);
      g_free (str);
    }

  else
    {
      str = g_strdup_printf ("%s (%s)",
			     pd->canvas_node->node->name->str,
			     pd->canvas_node->node->numeric_name->str);
      gtk_label_set_text (label, str);
      g_free (str);
    }


  label = (GtkLabel *) lookup_widget (GTK_WIDGET (pd->node_popup), "accumulated");
  str = g_strdup_printf ("Acummulated bytes: %g",
			 pd->canvas_node->node->accumulated);
  gtk_label_set_text (label, str);
  g_free (str);

  label = (GtkLabel *) lookup_widget (GTK_WIDGET (pd->node_popup), "average");
  str = g_strdup_printf ("Average bps: %g",
			 pd->canvas_node->node->average);
  gtk_label_set_text (label, str);
  g_free (str);

  gtk_widget_show (GTK_WIDGET (pd->node_popup));

  return FALSE;			/* Only called once */

}

static gint
node_item_event (GnomeCanvasItem * item, GdkEvent * event,
		 canvas_node_t * canvas_node)
{

  gdouble item_x, item_y;
  static struct popup_data pd =
  {NULL, NULL};
  static gint popup = 0;
  static GnomeAppBar *appbar;
  gchar *str;

  if (!appbar)
    appbar = GNOME_APPBAR (lookup_widget (GTK_WIDGET (app1), "appbar1"));

  /* This is not used yet, but it will be. */
  item_x = event->button.x;
  item_y = event->button.y;
  gnome_canvas_item_w2i (item->parent, &item_x, &item_y);

  switch (event->type)
    {

    case GDK_ENTER_NOTIFY:
      pd.canvas_node = canvas_node;
      popup = gtk_timeout_add (1000, (GtkFunction) popup_to, &pd);
      str = g_strdup_printf ("%s (%s)",
			     canvas_node->node->name->str,
			     canvas_node->node->numeric_name->str);
      gnome_appbar_push (appbar, str);
      g_free (str);
      break;
    case GDK_LEAVE_NOTIFY:
      if (popup)
	{
	  gtk_timeout_remove (popup);
	  popup = 0;
	  pd.canvas_node = NULL;
	  pd.node_popup = NULL;
	}
      gnome_appbar_pop (appbar);
      break;
    default:
      break;
    }

  return FALSE;

}

gint
reposition_canvas_nodes (guint8 * ether_addr, canvas_node_t * canvas_node,
			 GtkWidget * canvas)
{
  static gfloat angle = 0.0;
  static guint node_i = 0, n_nodes = 0;
  gdouble x, y, xmin, ymin, xmax, ymax, rad_max, text_compensation = 50;
  gdouble oddAngle = angle;

  gnome_canvas_get_scroll_region (GNOME_CANVAS (canvas),
				  &xmin,
				  &ymin,
				  &xmax,
				  &ymax);
  if (!n_nodes)
    {
      n_nodes = node_i = g_tree_nnodes (canvas_nodes);
    }

  xmin += text_compensation;
  xmax -= text_compensation;	/* Reduce the drawable area so that
				 * the node name is not lost
				 * TODO: Need a function to calculate
				 * text_compensation depending on font size */
  rad_max = ((xmax - xmin) > (ymax - ymin)) ? 0.9 * (y = (ymax - ymin)) / 2 : 0.9 *
    (x = (xmax - xmin)) / 2;

  if (n_nodes % 2 == 0)		/* spacing is better when n_nodes is odd and Y is linear */
    oddAngle = (angle * n_nodes) / (n_nodes + 1);
  if (n_nodes > 8)
    {
      x = rad_max * cos (oddAngle);
      y = rad_max * asin (sin (oddAngle)) / (M_PI / 2);
    }
  else
    {
      x = rad_max * cos (angle);
      y = rad_max * sin (angle);
    }

  gnome_canvas_item_set (GNOME_CANVAS_ITEM (canvas_node->group_item),
			 "x", x,
			 "y", y,
			 NULL);

  /* We update the text font */
  gnome_canvas_item_set (canvas_node->text_item,
			 "font", fontname,
			 NULL);

  if (diagram_only)
    {
      gnome_canvas_item_hide (canvas_node->text_item);
    }
  else
    {
      gnome_canvas_item_show (canvas_node->text_item);
      gnome_canvas_item_request_update (canvas_node->text_item);
      gnome_canvas_item_request_update (canvas_node->node_item);

    }

  node_i--;

  if (node_i)
    {
      angle += 2 * M_PI / n_nodes;
    }
  else
    {
      angle = 0.0;
      n_nodes = 0;
    }

  return FALSE;
}				/* reposition_canvas_nodes */


gint
update_canvas_links (guint8 * link_id, canvas_link_t * canvas_link,
		     GtkWidget * canvas)
{
  link_t *link;
  GnomeCanvasPoints *points;
  canvas_node_t *canvas_node;
  GtkArg args[2];
  gdouble link_size;
  struct timeval diff;
  guint32 scaledColor;
  gdouble scale;

  link = canvas_link->link;


  /* First we check whether the link has timed out */

  if (link->packets)
    update_packet_list (link->packets, LINK);

  diff = substract_times (now, link->last_time);

  if (link->n_packets == 0)
    {

      if (((diff.tv_sec * 1000000 + diff.tv_usec) > link_timeout_time)
	  && link_timeout_time)
	{

	  gtk_object_destroy (GTK_OBJECT (canvas_link->link_item));

	  g_tree_remove (canvas_links, link_id);

#if 0				/* TODO make proper debugging output */
	  if (interape)
	    g_log (G_LOG_DOMAIN, G_LOG_LEVEL_DEBUG,
		   _
	       ("Removing link and canvas_link: %s-%s. Number of links %d"),
		   ip_to_str (link_id), ip_to_str (link_id + 4),
		   g_tree_nnodes (canvas_links));
	  else
	    g_log (G_LOG_DOMAIN, G_LOG_LEVEL_DEBUG,
		   _
	       ("Removing link and canvas_link: %s-%s. Number of links %d"),
		   get_ether_name (link_id + 6), get_ether_name (link_id),
		   g_tree_nnodes (canvas_links));
#endif


	  /* TODO I have to create a free_link function in capture.c */
	  link_id = link->link_id;	/* Since we are freeing the link
					 * we must free its members as well 
					 * but if we free the id then we will
					 * not be able to find the link again 
					 * to free it, thus the intermediate variable */
	  if (link->main_prot)
	    g_free (link->main_prot);
	  g_free (link);
	  g_tree_remove (links, link_id);
	  g_free (link_id);

	  return TRUE;		/* I've checked it's not safe to traverse 
				 * while deleting, so we return TRUE to stop
				 * the traversion (Does that word exist? :-) */
	}
      else
	link->packets = NULL;

      link->accumulated = 0;
    }

  if (link && link->main_prot)
    gdk_color_parse (get_prot_color (link->main_prot), &canvas_link->color);

  args[0].name = "x";
  args[1].name = "y";

  points = gnome_canvas_points_new (2);

  /* If either source or destination has disappeared, we hide the link
   * until it can be show again */
  /* TODO: This is a dirty hack. Redo this again later by properly 
   * deleting the link */

  /* We get coords from source node */
  canvas_node = g_tree_lookup (canvas_nodes, link_id);
  if (!canvas_node)
    {
      gnome_canvas_item_hide (canvas_link->link_item);
      return FALSE;
    }
  gtk_object_getv (GTK_OBJECT (canvas_node->group_item), 2, args);
  points->coords[0] = args[0].d.double_data;
  points->coords[1] = args[1].d.double_data;

  /* And then for the destination node */
  canvas_node = g_tree_lookup (canvas_nodes, link_id + node_id_length);
  if (!canvas_node)
    {
      gnome_canvas_item_hide (canvas_link->link_item);
      return FALSE;
    }
  gtk_object_getv (GTK_OBJECT (canvas_node->group_item), 2, args);
  points->coords[2] = args[0].d.double_data;
  points->coords[3] = args[1].d.double_data;

  /* If we got this far, the link can be shown. Make sure it is */
  gnome_canvas_item_show (canvas_link->link_item);

  link_size = get_link_size (link->average);

  if (nofade)
    gnome_canvas_item_set (canvas_link->link_item,
			   "points", points,
			   "fill_color", get_prot_color (link->main_prot),
			   "width_units", link_size,
			   NULL);
  else
    {
      /* scale color down to 10% at link timeout */
      scale = pow (0.10, (diff.tv_sec * 1000000.0 + diff.tv_usec) / link_timeout_time);
      scaledColor = (((int) (scale * canvas_link->color.red) & 0xFF00) << 16) |
	(((int) (scale * canvas_link->color.green) & 0xFF00) << 8) |
	((int) (scale * canvas_link->color.blue) & 0xFF00) | 0xFF;
      gnome_canvas_item_set (canvas_link->link_item,
			     "points", points,
			     "fill_color_rgba", scaledColor,
			     "width_units", link_size, NULL);
    }

  gnome_canvas_points_unref (points);

  return FALSE;

}				/* update_canvas_links */

gint
update_canvas_nodes (guint8 * node_id, canvas_node_t * canvas_node,
		     GtkWidget * canvas)
{
  node_t *node;
  gdouble node_size;
  struct timeval diff;
  GtkArg args[1];

  node = canvas_node->node;

  /* First we check whether the link has timed out */

  if (node->packets)
    update_packet_list (node->packets, NODE);

  if (node->n_packets == 0)
    {
      diff = substract_times (now, node->last_time);

      if (((diff.tv_sec * 1000000 + diff.tv_usec) > node_timeout_time)
	  && node_timeout_time)
	{

	  gtk_object_destroy (GTK_OBJECT (canvas_node->group_item));

	  g_tree_remove (canvas_nodes, node_id);

#if 0				/* TODO redo */
	  if (interape)
	    g_log (G_LOG_DOMAIN, G_LOG_LEVEL_DEBUG,
		   _
		   ("Removing node and canvas_node: %s. Number of node %d"),
		   ip_to_str (node_id), g_tree_nnodes (canvas_nodes));
	  else
	    g_log (G_LOG_DOMAIN, G_LOG_LEVEL_DEBUG,
		   _
		   ("Removing node and canvas_node: %s. Number of nodes %d"),
		   get_ether_name (node_id), g_tree_nnodes (canvas_nodes));
#endif


	  /* TODO I have to create a free_node and a free_link function
	   * in capture.c */
	  node_id = node->node_id;	/* Since we are freeing the node
					 * we must free its members as well 
					 * but if we free the id then we will
					 * not be able to find the link again 
					 * to free it, thus the intermediate variable */
	  g_string_free (node->name, TRUE);
	  g_string_free (node->numeric_name, TRUE);
	  if (node->numeric_ip)
	    g_string_free (node->numeric_ip, TRUE);
	  g_free (node);
	  g_tree_remove (nodes, node_id);
	  g_free (node_id);

	  need_reposition = 1;

	  return TRUE;		/* I've checked it's not safe to traverse 
				 * while deleting, so we return TRUE to stop
				 * the traversion (Does that word exist? :-) */
	}
      else
	{
	  node->packets = NULL;
	  node->accumulated = 0;	/* TODO: do we really need this here anymore? */
	}
    }


  node_size = get_node_size (node->average);

  gnome_canvas_item_set (canvas_node->node_item,
			 "x1", -node_size / 2,
			 "x2", node_size / 2,
			 "y1", -node_size / 2,
			 "y2", node_size / 2,
			 NULL);

  /* We check the name of the node, and update the canvas node name
   * if it has changed (useful for non blocking dns resolving) */
  args[0].name = "text";
  gtk_object_getv (GTK_OBJECT (canvas_node->text_item), 1, args);
  if (strcmp (args[0].d.string_data, node->name->str))
    {
      gnome_canvas_item_set (canvas_node->text_item,
			     "text", node->name->str,
			     NULL);
      gnome_canvas_item_request_update (canvas_node->text_item);
    }

  return FALSE;			/* False means keep on calling the function */

}				/* update_canvas_nodes */

gint
check_new_link (guint8 * link_id, link_t * link, GtkWidget * canvas)
{
  canvas_link_t *new_canvas_link;
  GnomeCanvasGroup *group;
  GnomeCanvasPoints *points;

  GtkArg args[2];
  args[0].name = "x";
  args[1].name = "y";



  if (!g_tree_lookup (canvas_links, link_id))
    {
      group = gnome_canvas_root (GNOME_CANVAS (canvas));

      new_canvas_link = g_malloc (sizeof (canvas_link_t));
      new_canvas_link->canvas_link_id = link_id;
      new_canvas_link->link = link;

      /* We set the lines position using groups positions */
      points = gnome_canvas_points_new (2);

      points->coords[0] = points->coords[1] = points->coords[2] =
	points->coords[3] = 0.0;

      new_canvas_link->link_item = gnome_canvas_item_new (group,
					      gnome_canvas_line_get_type (),
							  "points", points,
						   "fill_color", link_color,
							  "width_units", 0.0,
							  NULL);


      g_tree_insert (canvas_links, link_id, new_canvas_link);
      gnome_canvas_item_lower_to_bottom (new_canvas_link->link_item);

      gnome_canvas_points_unref (points);

      gtk_signal_connect (GTK_OBJECT (new_canvas_link->link_item), "event",
			  (GtkSignalFunc) link_item_event, new_canvas_link);


/* TODO properly give link creating message */
#if 0
      g_log (G_LOG_DOMAIN, G_LOG_LEVEL_DEBUG,
	     _ ("Creating canvas_link: %s-%s. Number of links %d"),
	     (node_t
	      *) (g_tree_lookup (nodes,
			       new_canvas_link->canvas_link_id))->name->str,
	     (node_t
	      *) (g_tree_lookup (nodes,
				 (new_canvas_link->canvas_link_id) +
				 node_id_length))->name->str,
	     g_tree_nnodes (canvas_links));
#endif

    }

  return FALSE;
}				/* check_new_link */


/* Checks if there is a canvas_node per each node. If not, one canvas_node
 * must be created and initiated */
gint
check_new_node (guint8 * node_id, node_t * node, GtkWidget * canvas)
{
  canvas_node_t *new_canvas_node;
  GnomeCanvasGroup *group;

  if (!g_tree_lookup (canvas_nodes, node_id))
    {
      group = gnome_canvas_root (GNOME_CANVAS (canvas));

      new_canvas_node = g_malloc (sizeof (canvas_node_t));
      new_canvas_node->canvas_node_id = node_id;
      new_canvas_node->node = node;

      group = GNOME_CANVAS_GROUP (gnome_canvas_item_new (group,
					     gnome_canvas_group_get_type (),
							 "x", 100.0,
							 "y", 100.0,
							 NULL));

      new_canvas_node->node_item = gnome_canvas_item_new (group,
						  GNOME_TYPE_CANVAS_ELLIPSE,
							  "x1", 0.0,
							  "x2", 0.0,
							  "y1", 0.0,
							  "y2", 0.0,
							  "fill_color",
							  node_color,
							  "outline_color",
							  "black",
							  "width_pixels", 0,
							  NULL);
      new_canvas_node->text_item =
	gnome_canvas_item_new (group, GNOME_TYPE_CANVAS_TEXT, "text",
			       node->name->str,
			       "x", 0.0,
			       "y", 0.0,
			       "anchor", GTK_ANCHOR_CENTER,
			       "font", fontname,
			       "fill_color", text_color,
			       NULL);
      new_canvas_node->group_item = group;

      gnome_canvas_item_raise_to_top (GNOME_CANVAS_ITEM
				      (new_canvas_node->text_item));
      gtk_signal_connect (GTK_OBJECT (new_canvas_node->group_item), "event",
			  (GtkSignalFunc) node_item_event, new_canvas_node);

      g_tree_insert (canvas_nodes, node_id, new_canvas_node);
      g_log (G_LOG_DOMAIN, G_LOG_LEVEL_DEBUG,
	     _ ("Creating canvas_node: %s. Number of nodes %d"),
	     new_canvas_node->node->name->str, g_tree_nnodes (canvas_nodes));

      need_reposition = 1;

    }

  return FALSE;
}				/* check_new_node */


void
check_new_protocol (protocol_t * protocol, GtkWidget * canvas)
{
  GtkWidget *prot_table;
  GtkWidget *label;
  GtkArg args[2];
  GdkColor prot_color;
  GtkStyle *style;
  gchar *color_string;
  guint n_rows, n_columns;
  static gboolean first = TRUE;

  args[0].name = "n_rows";
  args[1].name = "n_columns";

  /* First, we check whether the diagram already knows about this protocol,
   * checking whether it is shown on the legend. */
  if (lookup_widget (GTK_WIDGET (canvas), protocol->name))
    return;

  /* It's not, so we build a new entry on the legend */
  /* First, we add a new row to the table */
  prot_table = lookup_widget (GTK_WIDGET (canvas), "prot_table");
  gtk_object_getv (GTK_OBJECT (prot_table), 2, args);
  n_rows = args[0].d.int_data;
  n_columns = args[0].d.int_data;
  /* Glade won't let me define a 0 row table
   * I feel this is ugly, but it's late and I don't feel like
   * cleaning this up :-) */
  if (!first)
    {
      gtk_table_resize (GTK_TABLE (prot_table), n_rows + 1, n_columns);
      n_rows++;
    }
  first = FALSE;

  /* Then we add the new label widgets */
  label = gtk_label_new (protocol->name);
  gtk_widget_ref (label);
  /* I'm not really sure what this exactly does. I just copied it from 
   * interface.c, but what I'm trying to do is set the name of the widget */
  gtk_object_set_data_full (GTK_OBJECT (app1), protocol->name, label,
			    (GtkDestroyNotify) gtk_widget_unref);


  gtk_widget_show (label);
  gtk_label_set_justify (GTK_LABEL (label), GTK_JUSTIFY_CENTER);
  gtk_table_attach (GTK_TABLE (prot_table), label,
		    0, 1, n_rows - 1, n_rows,
		    (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
		    (GtkAttachOptions) (GTK_EXPAND), 0, 0);

  color_string = get_prot_color (protocol->name);
  gdk_color_parse (color_string, &prot_color);
  gdk_colormap_alloc_color (gtk_widget_get_colormap (label), &prot_color,
			    TRUE, TRUE);

  style = gtk_style_new ();
  style->fg[GTK_STATE_NORMAL] = prot_color;
  gtk_widget_set_style (label, style);

}

/* Refreshes the diagram. Called each refresh_period ms
 * 1. Checks for new protocols and displays them
 * 2. Updates nodes looks
 * 3. Updates links looks
 */
guint
update_diagram (GtkWidget * canvas)
{
  static GnomeAppBar *appbar = NULL;
  guint n_links = 0, n_links_new = 1, n_protocols_new;
  guint n_nodes_before = 0, n_nodes_after = 1;
  static guint n_protocols = 0;
  gchar *str;


  g_mem_profile ();

  gettimeofday (&now, NULL);

  /* We search for new protocols */
  if (n_protocols != (n_protocols_new = g_list_length (protocols)))
    {
      g_list_foreach (protocols, (GFunc) check_new_protocol, canvas);
      n_protocols = n_protocols_new;
    }

/* Now we update the status bar with the number of present nodes 
 * TODO Find a nice use for the status bar. I can thik of very little */
  if (!appbar)
    appbar = GNOME_APPBAR (lookup_widget (GTK_WIDGET (canvas), "appbar1"));


  /* Check if there are any new nodes */
  g_tree_traverse (nodes, (GTraverseFunc) check_new_node, G_IN_ORDER, canvas);


  /* Update nodes look and delete outdated nodes */
  do
    {
      n_nodes_before = g_tree_nnodes (nodes);
      g_tree_traverse (canvas_nodes,
		       (GTraverseFunc) update_canvas_nodes,
		       G_IN_ORDER, canvas);
      n_nodes_after = g_tree_nnodes (nodes);
    }
  while (n_nodes_before != n_nodes_after);

  /* Reposition canvas_nodes and update status bar if a node has been
   * added or deleted */

  if (need_reposition)
    {
      g_tree_traverse (canvas_nodes,
		       (GTraverseFunc) reposition_canvas_nodes,
		       G_IN_ORDER, canvas);
      gnome_appbar_pop (appbar);

      str = g_strdup_printf (_ ("Number of nodes: %d"), n_nodes_after);
      gnome_appbar_push (appbar, str);
      g_free (str);

      need_reposition = 0;
    }


  /* Check if there are any new links */
  g_tree_traverse (links, (GTraverseFunc) check_new_link, G_IN_ORDER, canvas);

  /* Update links look 
   * We also delete timedout links, and when we do that we stop
   * traversing, so we need to go on until we have finished updating */

  do
    {
      n_links = g_tree_nnodes (links);
      g_tree_traverse (canvas_links,
		       (GTraverseFunc) update_canvas_links,
		       G_IN_ORDER, canvas);
      n_links_new = g_tree_nnodes (links);
    }
  while (n_links != n_links_new);


  return TRUE;			/* Keep on calling this function */

}				/* update_diagram */

void
init_diagram ()
{
  GtkWidget *widget;
  GtkSpinButton *spin;
  GtkStyle *style;
  GtkWidget *canvas;
  GdkColor color;

  /* Creates trees */
  canvas_nodes = g_tree_new (node_id_compare);
  canvas_links = g_tree_new (link_id_compare);


  /* Updates controls from values of variables */
  widget = lookup_widget (GTK_WIDGET (diag_pref), "node_radius_slider");
  gtk_adjustment_set_value (GTK_RANGE (widget)->adjustment,
			    log (node_radius_multiplier) / log (10));
  gtk_signal_emit_by_name (GTK_OBJECT (GTK_RANGE (widget)->adjustment),
			   "changed");
  widget = lookup_widget (GTK_WIDGET (diag_pref), "link_width_slider");
  gtk_adjustment_set_value (GTK_RANGE (widget)->adjustment,
			    log (link_width_multiplier) / log (10));
  gtk_signal_emit_by_name (GTK_OBJECT (GTK_RANGE (widget)->adjustment),
			   "changed");
  spin =
    GTK_SPIN_BUTTON (lookup_widget (GTK_WIDGET (diag_pref), "averaging_spin"));
  gtk_spin_button_set_value (spin, averaging_time / 1000);
  spin = GTK_SPIN_BUTTON (lookup_widget (GTK_WIDGET (diag_pref), "refresh_spin"));
  gtk_spin_button_set_value (spin, refresh_period);
  spin = GTK_SPIN_BUTTON (lookup_widget (GTK_WIDGET (diag_pref), "node_to_spin"));
  gtk_spin_button_set_value (spin, node_timeout_time / 1000);
  spin = GTK_SPIN_BUTTON (lookup_widget (GTK_WIDGET (diag_pref), "link_to_spin"));
  gtk_spin_button_set_value (spin, link_timeout_time / 1000);

  widget = lookup_widget (GTK_WIDGET (diag_pref), "diagram_only_toggle");
  gtk_toggle_button_set_active (widget, diagram_only);

  /* Connects signals */
  widget = lookup_widget (diag_pref, "node_radius_slider");
  gtk_signal_connect (GTK_OBJECT (GTK_RANGE (widget)->adjustment),
		      "value_changed",
		      GTK_SIGNAL_FUNC
		      (on_node_radius_slider_adjustment_changed), NULL);
  widget = lookup_widget (diag_pref, "link_width_slider");
  gtk_signal_connect (GTK_OBJECT (GTK_RANGE (widget)->adjustment),
		      "value_changed",
		      GTK_SIGNAL_FUNC
		      (on_link_width_slider_adjustment_changed), NULL);
  widget = lookup_widget (diag_pref, "averaging_spin");
  gtk_signal_connect (GTK_OBJECT (GTK_SPIN_BUTTON (widget)->adjustment),
		      "value_changed",
		      GTK_SIGNAL_FUNC
		      (on_averaging_spin_adjustment_changed), NULL);
  widget = lookup_widget (diag_pref, "refresh_spin");
  gtk_signal_connect (GTK_OBJECT (GTK_SPIN_BUTTON (widget)->adjustment),
		      "value_changed",
		      GTK_SIGNAL_FUNC
		      (on_refresh_spin_adjustment_changed),
		      lookup_widget (GTK_WIDGET (app1), "canvas1"));
  widget = lookup_widget (diag_pref, "node_to_spin");
  gtk_signal_connect (GTK_OBJECT (GTK_SPIN_BUTTON (widget)->adjustment),
		      "value_changed",
		      GTK_SIGNAL_FUNC
		      (on_node_to_spin_adjustment_changed), NULL);
  widget = lookup_widget (diag_pref, "link_to_spin");
  gtk_signal_connect (GTK_OBJECT (GTK_SPIN_BUTTON (widget)->adjustment),
		      "value_changed",
		      GTK_SIGNAL_FUNC
		      (on_link_to_spin_adjustment_changed), NULL);
  widget = lookup_widget (diag_pref, "size_mode_menu");
  gtk_signal_connect (GTK_OBJECT (GTK_OPTION_MENU (widget)->menu),
		 "deactivate", GTK_SIGNAL_FUNC (on_size_mode_menu_selected),
		      NULL);



  /* Sets canvas background to black */
  canvas = lookup_widget (GTK_WIDGET (app1), "canvas1");
  gdk_color_parse ("black", &color);

  gdk_colormap_alloc_color (gtk_widget_get_colormap (canvas), &color, TRUE,
			    TRUE);
  style = gtk_style_new ();
  style->bg[GTK_STATE_NORMAL] = color;
  gtk_widget_set_style (canvas, style);

  gtk_style_set_background (canvas->style, canvas->window, GTK_STATE_NORMAL);

  /* Since glade doesn't do it, we'll have to do it manually 
   * TODO Remove when glade is fixed */

#if 0
  widget = lookup_widget (GTK_WIDGET (app1), "appbar1");
  gnome_app_install_appbar_menu_hints (widget, &(view1_menu_uiinfo[0]));

  widget = lookup_widget (GTK_WIDGET (app1), "toolbar_check");
  gtk_menu_item_select (widget);
#endif
}
