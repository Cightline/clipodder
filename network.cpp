
/* Mostly libcurl stuff */

//#include "clipodder.hpp"

#include "network.hpp"


struct buffer_struct
{
  const char *filename;
  FILE *stream;

};

static int curl_write(void *buffer, size_t size, size_t nmemb, void *data)
{
  //https://github.com/akrennmair/newsbeuter/blob/master/rss/parser.cpp                                       
  /* cast to std::string */
  std::string *pbuf = static_cast<std::string *>(data);

  pbuf->append(static_cast<const char *>(buffer), size *nmemb);
  return size * nmemb;
}



static int curl_write_file(void *ptr, size_t size, size_t nmemb, void *userdata)
{
  buffer_struct *t_struct = (buffer_struct *)userdata;
  
  if (t_struct && !t_struct->stream)
    {
      t_struct->stream = fopen(t_struct->filename, "wb"); 
    }


  if (!t_struct->stream)
    {
      return -1;
    }

  return fwrite(ptr, size, nmemb, t_struct->stream);
}


std::string *network::fetch_page(std::string url)
{
  std::string *buf = new std::string;
  CURL *curl;
  CURLcode res;

  curl = curl_easy_init();
 
  if(curl)
    {
      curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
      curl_easy_setopt(curl, CURLOPT_VERBOSE, debug::state);
      curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, curl_write);
      curl_easy_setopt(curl, CURLOPT_WRITEDATA, buf);
      curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1);
      curl_easy_setopt(curl, CURLOPT_MAXREDIRS, 5);
      //curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 1);
      curl_easy_setopt(curl, CURLOPT_LOW_SPEED_LIMIT, 5);
      curl_easy_setopt(curl, CURLOPT_LOW_SPEED_TIME, 10);
      res = curl_easy_perform(curl);

      if (res == 0)
	{
	  curl_easy_cleanup(curl);
	}
    }
    
  return buf;
}
 
 
int network::download_file(std::string url, std::string download_path)
{
  
  CURL *curl;
  CURLcode res;
  const char *dl_path = download_path.c_str();
  

  struct buffer_struct d_struct;
  
  d_struct.filename = dl_path;
  d_struct.stream = NULL;


  curl = curl_easy_init();
  
  if (curl)
    {
      curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
      curl_easy_setopt(curl, CURLOPT_VERBOSE, debug::state);
      curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, curl_write_file);
      curl_easy_setopt(curl, CURLOPT_WRITEDATA, &d_struct);
      curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1);
      curl_easy_setopt(curl, CURLOPT_MAXREDIRS, 5);
      //curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 1);
      curl_easy_setopt(curl, CURLOPT_LOW_SPEED_LIMIT, 5);
      curl_easy_setopt(curl, CURLOPT_LOW_SPEED_TIME, 10);
      res = curl_easy_perform(curl);
      
      if (res == 0)
	{
	  curl_easy_cleanup(curl);
	  /* Fix this */
	  fclose(d_struct.stream);
	}
      
    }
  

  
  return res;
}
