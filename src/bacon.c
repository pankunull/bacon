/*
 * We love Bacon !
 * Simple HTTP beacon
 */

#include <stdio.h>
#include <string.h>
#include <sys/utsname.h>
#include <time.h>
#include <unistd.h>
#include <curl/curl.h>

#define REPORT_SIZE 2048

/*
 * Time function
 */
const char
*get_time_iso(void)
{
	static char buf[64];
	time_t now = time(NULL);

	strftime(buf, sizeof(buf),
	     "%Y-%m-%dT%H:%M:%SZ",
	     gmtime(&now));

	return buf;
}


/*
 * Data to send over
 */
void
build_report(char *out, size_t size,
                  const char *host,
                  const char *kernel,
                  const char *msg)
{
    snprintf(out, size,
             "host: %s\n"
             "kernel: %s\n"
             "time: %s\n"
             "message: %s\n",
             host, kernel, get_time_iso(), msg);
}


/*
 * POST
 */
int
http_post(const char *url,
		const char *user,
		const char *pass,
		const char *data)
{
	CURL *curl = curl_easy_init();
	if (!curl) return -1;

	char auth[256];
	snprintf(auth, sizeof(auth), "%s:%s", user, pass);

	struct curl_slist *headers = NULL;
	headers = curl_slist_append(headers, "Content-Type: text/plain");

	// Basic auth
	//curl_easy_setopt(curl, CURLOPT_USERPWD, auth);

	curl_easy_setopt(curl, CURLOPT_URL, url);
	curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
	curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data);

	curl_easy_setopt(curl, CURLOPT_USERAGENT, "bacon-agent/1.0");

	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);

	CURLcode r = curl_easy_perform(curl);

	curl_slist_free_all(headers);
	curl_easy_cleanup(curl);

	return (r == CURLE_OK) ? 0 : -1;
}


int
main()
{
	curl_global_init(CURL_GLOBAL_DEFAULT);

	char host[256];
	gethostname(host, sizeof(host));

	struct utsname u;
	uname(&u);

	char report[REPORT_SIZE];
	const char *msg = "We love bacon!";

	const char url[] = "https://url";
	// Only for auth_basic
	const char user[] = "user";
	const char pass[] = "password";

	build_report(report, sizeof(report), host, u.release, msg);

	printf("[*] Sending report...\n");

	http_post(url, user, pass, report);

	curl_global_cleanup();

	return 0;
}
