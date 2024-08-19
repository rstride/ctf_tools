#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <math.h>
#include <arpa/inet.h>
#include <time.h>

#define HOST "challenge01.root-me.org"
#define PORT 52002

int main() {
    int sock;
    struct sockaddr_in server_addr;
    char buffer[1024];
    float number1, number2, result;

    // Create socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket creation error");
        return 1;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);

    // Convert hostname to IP address
    if (inet_pton(AF_INET, HOST, &server_addr.sin_addr) <= 0) {
        perror("Invalid address/Address not supported");
        return 1;
    }

    // Connect to server
    if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Connection failed");
        return 1;
    }

    // Read data from server
    read(sock, buffer, sizeof(buffer));
    sscanf(buffer, "%f %f", &number1, &number2);
    printf("Received: %f %f\n", number1, number2);

    // Start timer
    clock_t start_time = clock();

    // Calculate result
    result = sqrt(number1) * number2;
    result = roundf(result * 100) / 100;

    // Convert result to string and send back to server
    snprintf(buffer, sizeof(buffer), "%.2f\n", result);
    write(sock, buffer, strlen(buffer));

    // Check if the operation was done within 2 seconds
    clock_t end_time = clock();
    double elapsed_time = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;
    if (elapsed_time > 2.0) {
        printf("Failed: Operation took too long (%.2f seconds)\n", elapsed_time);
    } else {
        printf("Success: Operation completed in %.2f seconds\n", elapsed_time);
    }

    // Read server response
    read(sock, buffer, sizeof(buffer));
    printf("Server response: %s\n", buffer);

    // Close the socket
    close(sock);

    return 0;
}