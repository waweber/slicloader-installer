/*
 *  SLICLoader Installer, automated installer for SLICLoader
 *  Copyright (C) 2013 wweber
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

typedef struct _KEY_ENTRY
{
	const char* Name;
	const char* Key;
} KEY_ENTRY;

typedef struct _SLIC_ENTRY
{
	const char* Name;
	int Size;
	const char* Slic;
} SLIC_ENTRY;

typedef struct _CERT_ENTRY
{
	const char* Name;
	int Size;
	const char* Cert;
} CERT_ENTRY;

//These need to be filled in.
KEY_ENTRY gKeyList[] =
{

};
int gKeyCount = 0;

//These need to be filled in.
SLIC_ENTRY gSlicList[] = 
{
	
};
int gSlicCount = 0;

//These need to be filled in.
CERT_ENTRY gCertList[] = 
{

};
int gCertCount = 0;

//These need to be filled in.
const unsigned char gLoaderData[] = {

};
const unsigned int gLoaderSize = 0;

