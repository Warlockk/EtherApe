/*
   Etherape
   Copyright (C) 2000 Juan Toledo

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
   
   ----------------------------------------------------------------
   
   dns wrapper routines
*/

/* initialize dns interface */
void dns_open (void);

/* close dns interface */
void dns_close(void);

/* returns 1 if the current dns implementation has a socket wich needs a select() */
int dns_hasfd(void);

/* returns the file descriptor associated with dns socket */
int dns_waitfd (void);

/* called when the dns_waitfd socket has available data */
void dns_ack (void);

/* resolves address and returns its fqdn (if the corresponding parameter is nonzero)
   or just the hostname (if fqdn is zero) */
char *dns_lookup (uint32_t address, int fqdn);

