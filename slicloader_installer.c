#include <stdio.h>
#include <stdlib.h>
#include "slicloader_installer.h"

int getKey()
{
	printf("Select a key from the following list.\nNote: Pick a key that matches your operating system version.\n");
	
	int i;
	for (i = 0; i < gKeyCount; ++i)
	{
		printf("  %d.  %s\n", i + 1, gKeyList[i].Name);
	}
	
	printf("\n");
	
	unsigned int key;
	int res;
	do
	{
		printf("Key ID: ");
		res = scanf("%u", &key);
		key -= 1;
	}
	while (key >= gKeyCount);
	
	return key;
}

int getSlic()
{
	printf("Select a SLIC table from the following list:\nNote: The make should be the same as the key you chose.\n");
	
	int i;
	for (i = 0; i < gSlicCount; ++i)
	{
		printf("  %d.  %s\n", i + 1, gSlicList[i].Name);
	}
	
	printf("\n");
	
	unsigned int slic;
	int res;
	do
	{
		printf("SLIC ID: ");
		res = scanf("%d", &slic);
		slic -= 1;
	}
	while (slic >= gSlicCount);
	
	return slic;
}

int getCert()
{
	printf("Select a certificate from the following list:\nNote: The make should match the SLIC and key you chose.\n");
	
	int i;
	for (i = 0; i < gCertCount; ++i)
	{
		printf("  %d.  %s\n", i + 1, gCertList[i].Name);
	}
	
	printf("\n");
	
	unsigned int cert;
	int res;
	do
	{
		printf("Certificate ID: ");
		res = scanf("%d", &cert);
		cert -= 1;
	}
	while (cert  >= gCertCount);
	
	return cert;
}

int getOperation()
{
	printf("Choose operation (\"install\" or \"uninstall\")\n");
	char op[128];
	int res;
	
	do
	{
		printf("Operation: ");
		res = scanf("%s", op);
	}
	while (strcmp(op, "install") != 0 && strcmp(op, "uninstall") != 0);
	
	if (strcmp(op, "install") == 0)
	{
		return 0;
	}
	else
	{
		return 1;
	}
}

int install()
{
	KEY_ENTRY key = gKeyList[getKey()];
	SLIC_ENTRY slic = gSlicList[getSlic()];
	CERT_ENTRY cert = gCertList[getCert()];
	
	int res;
	printf("Installing key [%s]...\n", key.Name);
	
	char buf[256];
	sprintf(buf, "slmgr -ipk %s", key.Key);
	res = system(buf);
	
	if (res)
	{
		printf("Installing key failed!\n");
		return 1;
	}
	
	printf("Installing cert [%s]...\n", cert.Name);
	
	FILE* f = fopen(".cert", "w+b");
	
	if (!f)
	{
		printf("Could not write certificate!\n");
		return 1;
	}
	
	fwrite(cert.Cert, 1, cert.Size, f);
	fclose(f);
	
	res = system("slmgr -ilc .cert");
	
	if (res)
	{
		res = system("DEL .cert");
		printf("Could not install certificate!\n");
		return 1;
	}
	
	res = system("DEL .cert");
	
	printf("Installing boot manager...\n");
	res = system("MOUNTVOL T: /S");
	
	if (res)
	{
		printf("Could not mount ESP volume! (is T:\\ in use?)\n");
		return 1;
	}
	
	//see if the .orig file exists
	f = fopen("T:\\EFI\\Microsoft\\Boot\\bootmgfw.orig.efi", "r");
	if (f == NULL)
	{
		//move it
		res = system("MOVE /Y T:\\EFI\\Microsoft\\Boot\\bootmgfw.efi T:\\EFI\\Microsoft\\Boot\\bootmgfw.orig.efi");
		
		if (res)
		{
			printf("Could not install boot manager!\n");
			res = system("MOUNTVOL T: /D");
			return 1;
		}
		
		//write new file
		f = fopen("T:\\EFI\\Microsoft\\Boot\\bootmgfw.efi", "w+b");
		
		if (!f)
		{
			printf("Could not write boot manager!\n");
			res = system("MOUNTVOL T: /D");
			return 1;
		}
		
		fwrite(gLoaderData, 1, gLoaderSize, f);
		fclose(f);
	}
	else
	{
		fclose(f);
		printf("Boot manager already installed.\n");
	}
	
	printf("Installing SLIC [%s]...\n", slic.Name);
	
	f = fopen("T:\\EFI\\Microsoft\\Boot\\slic.bin", "w+b");
	
	if (!f)
	{
		printf("Could not write SLIC!\n");
		res = system("MOUNTVOL T: /D");
		return 1;
	}
	
	fwrite(slic.Slic, 1, slic.Size, f);
	fclose(f);
	
	res = system("MOUNTVOL T: /D");
	
	printf("Done.\n");
	
	return 0;
}

int uninstall()
{
	printf("Restoring original boot manager...\n");
	
	int res;
	res = system("MOUNTVOL T: /S");
	
	if (res)
	{
		printf("Could not mount ESP volume!\n");
		return 1;
	}
	
	res = system("MOVE /Y T:\\EFI\\Microsoft\\Boot\\bootmgfw.orig.efi T:\\EFI\\Microsoft\\Boot\\bootmgfw.efi");
	if (res)
	{
		printf("Warning: No boot manager backup detected! Will attempt to write new one...\n");
		res = system("COPY /Y %SYSTEMROOT%\\Boot\\EFI\\bootmgfw.efi T:\\EFI\\Microsoft\\Boot\\bootmgfw.efi");
		if (res)
		{
			printf("That failed too! This is bad.\n");
		}
	}
	
	res = system("DEL T:\\EFI\\Microsoft\\Boot\\slic.bin");
	if (res)
	{
		printf("Failed to delete SLIC.\n");
	}
	
	res = system("MOUNTVOL T: /D");
	
	return 0;
}

int main(int argc, char** argv)
{
	printf("SLICLoader Installer\n\n");
	
	int op = getOperation();

	if (op == 0)
	{
		int res = install();
		if (res)
		{
			printf("\nThe installation failed.\n");
		}
		else
		{
			printf("\nThe installation succeeded.\n");
		}
	}
	else
	{
		int res = uninstall();
		if (res)
		{
			printf("\nThe uninstall failed.\n");
		}
		else
		{
			printf("\nUninstall complete.\n");
		}
	}
	
	int res = system("PAUSE");
	
	return 0;
}
