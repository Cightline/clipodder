
/* Mostly libcurl stuff */

//#include "clipodder.hpp"

#include "network.hpp"

static int previous_percent = 0;

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



static int progress(void *clientp, double dltotal, double dlnow, double ultotal, double ulnow)
{
 
    
  int percent = dlnow/dltotal * 100;
  std::string progress; 
  std::stringstream percent_str;
 

  if (output::suppress != 0)
  {
      return 0;
  }
  if (percent < 0)
    {
      return 0;
    }
 
  /* This is done so it looks like [  8], instead of [8] */
  if (percent < 10)
    {
        percent_str << percent;
        progress.append("[--");
        progress.append(percent_str.str());
        progress.append("]");
    }

  else if (percent > 9 && percent < 100)
    {
        percent_str << percent;
        progress.append("[-");
        progress.append(percent_str.str());
        progress.append("]");
    }

  else if (percent == 100)
    {
        progress.append("[100]");
    }
  
  else 
    {
        progress.append("[---]");
    }

  for (int i = 0; i < progress.length(); i++)
  {
      std::cout << "\b";
  }

  std::cout << progress << "\r\n" << std::flush;
  
  return 0;
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
      curl_easy_setopt(curl, CURLOPT_VERBOSE, 0);
      curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, curl_write);
      curl_easy_setopt(curl, CURLOPT_WRITEDATA, buf);
      curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1);
      curl_easy_setopt(curl, CURLOPT_MAXREDIRS, 5);
      curl_easy_setopt(curl, CURLOPT_LOW_SPEED_LIMIT, 5);
      curl_easy_setopt(curl, CURLOPT_LOW_SPEED_TIME, 10);

      res = curl_easy_perform(curl);

      if (res == 0)
	{
	 
	}
      curl_easy_cleanup(curl);
    }
    
  return buf;
}
 
 
int network::download_file(std::string url, std::string download_path, bool show_progress)
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
      curl_easy_setopt(curl, CURLOPT_VERBOSE, 0);
      curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, curl_write_file);
      curl_easy_setopt(curl, CURLOPT_WRITEDATA, &d_struct);
      curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1);
      curl_easy_setopt(curl, CURLOPT_MAXREDIRS, 5);
      curl_easy_setopt(curl, CURLOPT_LOW_SPEED_LIMIT, 5);
      curl_easy_setopt(curl, CURLOPT_LOW_SPEED_TIME, 10);

      if (show_progress)
	{
	  curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 0);
	  curl_easy_setopt(curl, CURLOPT_PROGRESSFUNCTION, progress);
	}
      
      res = curl_easy_perform(curl);
      
      if (res == 0)
	{
	  fclose(d_struct.stream);
	}
      
      curl_easy_cleanup(curl);
    }
  

  
  return res;
}
