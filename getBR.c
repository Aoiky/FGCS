#include <stdio.h>  
#include <ifaddrs.h>  
#include <arpa/inet.h>
#include <string.h>  

struct sockaddr_in getBR()
{
    struct sockaddr_in *sin = NULL, result;
    struct ifaddrs *ifa = NULL, *ifList;
	
    if (getifaddrs(&ifList) < 0)
    {
        return result;
    }

    for (ifa = ifList; ifa != NULL; ifa = ifa->ifa_next)
    {
        if(ifa->ifa_addr->sa_family == AF_INET)
        {
            //printf("\n>>> interfaceName: %s\n", ifa->ifa_name);

            sin = (struct sockaddr_in *)ifa->ifa_addr;
            //printf(">>> ipAddress: %s\n", inet_ntoa(sin->sin_addr));
			if ( strcmp(inet_ntoa(sin->sin_addr), "127.0.0.1") != 0)
			{
				sin = (struct sockaddr_in *)ifa->ifa_dstaddr;
				result = *sin;
				//printf(">>> broadcast: %s\n", inet_ntoa(sin->sin_addr));
				
				//sin = (struct sockaddr_in *)ifa->ifa_netmask;
				//printf(">>> subnetMask: %s\n", inet_ntoa(sin->sin_addr));
				break;
			}
            
        }
    }

    freeifaddrs(ifList);

    return result;
}

 /* 
int main(void)
{
    printf(">>> broadcast: %s\n", inet_ntoa(getSubnetMask().sin_addr));

    return 0;
}
*/