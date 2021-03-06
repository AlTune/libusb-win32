/* LIBUSB-WIN32, Generic Windows USB Library
 * Copyright (c) 2002-2005 Stephan Meyer <ste_meyer@web.de>
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
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */


#include "libusb_driver.h"



NTSTATUS release_interface(libusb_device_t *dev, FILE_OBJECT *file_object,
                           int interface)
{
  DEBUG_MESSAGE("release_interface(): interface %d", interface);

  if(!dev->config.value)
    {
      DEBUG_ERROR("release_interface(): device is not configured"); 
      return STATUS_INVALID_DEVICE_STATE;
    }

  if(interface >= LIBUSB_MAX_NUMBER_OF_INTERFACES)
    {
      DEBUG_ERROR("release_interface(): interface number %d too high", 
                  interface);
      return STATUS_INVALID_PARAMETER;
    }

  if(!dev->config.interfaces[interface].valid)
    {
      DEBUG_ERROR("release_interface(): invalid interface %02d", interface);
      return STATUS_INVALID_PARAMETER;
    }

  if(!dev->config.interfaces[interface].file_object)
    {
      DEBUG_ERROR("claim_interface(): could not release interface %d, "
                  "interface is not claimed", interface);
      return STATUS_INVALID_DEVICE_STATE;
    }

  if(dev->config.interfaces[interface].file_object != file_object)
    {
      DEBUG_ERROR("claim_interface(): could not release interface %d, "
                  "interface is not bound to this file object", interface);
      return STATUS_DEVICE_BUSY;
    }

  dev->config.interfaces[interface].file_object = NULL;

  return STATUS_SUCCESS;
}

NTSTATUS release_all_interfaces(libusb_device_t *dev, FILE_OBJECT *file_object)
{
  int i;
  
  DEBUG_MESSAGE("release_all_interfaces(): releasing all interfaces"
                " bound to file object 0x%x", file_object);

  for(i = 0; i < LIBUSB_MAX_NUMBER_OF_INTERFACES; i++)
    {
      if(dev->config.interfaces[i].file_object == file_object)
        {
          dev->config.interfaces[i].file_object = NULL;
        }
    }

  return STATUS_SUCCESS;
}
