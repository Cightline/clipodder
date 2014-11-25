Clipodder

Dependencies
	
all:
	
- gcc
- make
	
Arch:
- curl
- libxml2
- confuse

Debian:
- libcurl4-gnutls-dev or libcurl4-openssl-dev
- libxml2-dev
- libconfuse-dev
	



Installation

`make`
`make install` (with proper permissions)

	
Options:

The configuration should be located at ~/.clipodder/config 

format 
-----------------------------
The format that you want clipodder to download.  

(default all)
```
format = {"video","mp4"}
```
	
num_downloads 
-----------------------------
The number of downloads you want per "run". Don't set this higher than max_downloads 
or you will download, then subsequently delete the files. 
	
```
num_downloads = 4
```
	
max_downloads 
-----------------------------
The number of downloads you want in the directory. If you have num_downloads set to 2, it 
will download the latest 2 from the feed. If max_downlaods was set to 4, clipodder will
download 2, then see if it is exceeding 4 files, if so the extra files are removed. 

(default 1)
```
max_downloads = 5
```
	
no_child_dir 
-----------------------------
Set this to 1 to not use the title of the feed as the name of the target directory. 
When set to the default, the path should look something like "some_dl_dir/Title of Feed/video.mp4".
Otherwise it should look something like "some_dl_dir/video.mp4". 

(default no)
```
no_child_dir = no
```
	
download_dir  
-----------------------------	

The target parent directory to download to (unless no_child_dir is set). Can be used
globally or per url. 

(default ~/.clipodder/downloads) 
	
```
download_dir = "/var/lib/some_dir"
```

show_path 
----------------------------
Show the full path that clipodder is saving to.

(global) (default no)
```
show_path = no
```


show_progress 
----------------------------
Enables the percentage meter. (Does not seem to 
work correctly on Debian, but it does work on Arch)

(global) (default no)
```
show_progress = no
```
