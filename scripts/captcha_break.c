#include <stdio.h>
#include <stdlib.h>
#include <curl/curl.h>
#include <tesseract/capi.h>

// Structure for handling memory during curl operations
struct memory {
    char *response;
    size_t size;
};

// Callback function for curl to write data to memory
static size_t write_callback(void *data, size_t size, size_t nmemb, void *userp) {
    size_t realsize = size * nmemb;
    struct memory *mem = (struct memory *)userp;

    char *ptr = realloc(mem->response, mem->size + realsize + 1);
    if(ptr == NULL) {
        printf("Not enough memory!\n");
        return 0;
    }

    mem->response = ptr;
    memcpy(&(mem->response[mem->size]), data, realsize);
    mem->size += realsize;
    mem->response[mem->size] = 0;

    return realsize;
}

// Function to download the CAPTCHA image
char *download_captcha(const char *url) {
    CURL *curl;
    CURLcode res;
    struct memory chunk = {0};

    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();
    if(curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

        res = curl_easy_perform(curl);
        if(res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        }

        curl_easy_cleanup(curl);
    }

    curl_global_cleanup();
    return chunk.response;
}

// Function to perform OCR on the image and get the text
char *perform_ocr(const char *image_data, size_t image_size) {
    char *outText;
    TessBaseAPI *handle = TessBaseAPICreate();

    if(TessBaseAPIInit3(handle, NULL, "eng")) {
        fprintf(stderr, "Could not initialize tesseract.\n");
        TessBaseAPIDelete(handle);
        return NULL;
    }

    TessBaseAPISetImage(handle, (unsigned char*)image_data, image_size, 1, image_size);
    outText = TessBaseAPIGetUTF8Text(handle);

    TessBaseAPIDelete(handle);
    return outText;
}

// Function to send the CAPTCHA text back to the server
void send_captcha(const char *captcha) {
    CURL *curl;
    CURLcode res;
    char postdata[100];

    snprintf(postdata, sizeof(postdata), "captcha=%s", captcha);

    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();
    if(curl) {
        curl_easy_setopt(curl, CURLOPT_URL, "http://challenge01.root-me.org/programmation/ch8/");
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postdata);

        res = curl_easy_perform(curl);
        if(res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        }

        curl_easy_cleanup(curl);
    }

    curl_global_cleanup();
}

int main(void) {
    char *image_data = download_captcha("http://challenge01.root-me.org/programmation/ch8/");
    if (!image_data) {
        printf("Failed to download CAPTCHA.\n");
        return 1;
    }

    char *captcha_text = perform_ocr(image_data, strlen(image_data));
    if (!captcha_text) {
        printf("Failed to perform OCR.\n");
        free(image_data);
        return 1;
    }

    send_captcha(captcha_text);

    free(image_data);
    free(captcha_text);

    return 0;
}