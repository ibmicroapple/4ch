static Thread *parse_thread(cJSON *node);
static void free_thread(Thread *thread);
static Post *parse_post(cJSON *node);
static void free_post(Post *post);
static int get_html(const char *board, const uint64_t id);

static Thread *parse_thread(cJSON *node)
{
	Thread *thread;
	cJSON *subitem;

	thread = malloc(sizeof(Thread));
	if(!thread) return(0);
	thread->no = thread->last_modified = 0;

	subitem = cJSON_GetObjectItem(node, "no");
	if(subitem) thread->no = subitem->valueint;

	subitem = cJSON_GetObjectItem(node, "last_modified");
	if(subitem) thread->last_modified = subitem->valueint;

	return(thread);
}

static void free_thread(Thread *thread)
{
	free(thread);
}

static Post* parse_post(cJSON *node)
{
	Post *post;
	cJSON *subitem;

	post = malloc(sizeof(Post));
	if(!post) return(0);
	*post = (Post){0};

	subitem = cJSON_GetObjectItem(node, "no");
	if(subitem) post->no = subitem->valueint;

	subitem = cJSON_GetObjectItem(node, "resto");
	if(subitem) post->resto = subitem->valueint;

	subitem = cJSON_GetObjectItem(node, "sticky");
	if(subitem) post->sticky = subitem->valueint;

	subitem = cJSON_GetObjectItem(node, "closed");
	if(subitem) post->closed = subitem->valueint;

	subitem = cJSON_GetObjectItem(node, "archived");
	if(subitem) post->archived = subitem->valueint;

	subitem = cJSON_GetObjectItem(node, "archived_on");
	if(subitem) post->archived_on = subitem->valueint;

	subitem = cJSON_GetObjectItem(node, "now");
	if(subitem) string_set(&post->now, subitem->valuestring);

	subitem = cJSON_GetObjectItem(node, "time");
	if(subitem) post->time = subitem->valueint;

	subitem = cJSON_GetObjectItem(node, "name");
	if(subitem) string_set(&post->name, subitem->valuestring);

	subitem = cJSON_GetObjectItem(node, "trip");
	if(subitem) string_set(&post->trip, subitem->valuestring);

	subitem = cJSON_GetObjectItem(node, "id");
	if(subitem) string_set(&post->id, subitem->valuestring);

	subitem = cJSON_GetObjectItem(node, "capcode");
	if(subitem) string_set(&post->capcode, subitem->valuestring);

	subitem = cJSON_GetObjectItem(node, "country");
	if(subitem) string_set(&post->country, subitem->valuestring);

	subitem = cJSON_GetObjectItem(node, "country_name");
	if(subitem) string_set(&post->country_name, subitem->valuestring);

	subitem = cJSON_GetObjectItem(node, "sub");
	if(subitem) string_set(&post->sub, subitem->valuestring);

	subitem = cJSON_GetObjectItem(node, "com");
	if(subitem) string_set(&post->com, subitem->valuestring);

	subitem = cJSON_GetObjectItem(node, "tim");
	if(subitem) post->tim = subitem->valueint;

	subitem = cJSON_GetObjectItem(node, "filename");
	if(subitem) string_set(&post->filename, subitem->valuestring);

	subitem = cJSON_GetObjectItem(node, "ext");
	if(subitem) string_set(&post->ext, subitem->valuestring);

	subitem = cJSON_GetObjectItem(node, "fsize");
	if(subitem) post->fsize = subitem->valueint;

	subitem = cJSON_GetObjectItem(node, "md5");
	if(subitem) string_set(&post->md5, subitem->valuestring);

	subitem = cJSON_GetObjectItem(node, "w");
	if(subitem) post->w = subitem->valueint;

	subitem = cJSON_GetObjectItem(node, "h");
	if(subitem) post->h = subitem->valueint;

	subitem = cJSON_GetObjectItem(node, "tn_w");
	if(subitem) post->tn_w = subitem->valueint;

	subitem = cJSON_GetObjectItem(node, "tn_h");
	if(subitem) post->tn_h = subitem->valueint;

	subitem = cJSON_GetObjectItem(node, "filedeleted");
	if(subitem) post->filedeleted = subitem->valueint;

	subitem = cJSON_GetObjectItem(node, "spoiler");
	if(subitem) post->spoiler = subitem->valueint;

	subitem = cJSON_GetObjectItem(node, "custom_spoiler");
	if(subitem) post->custom_spoiler = subitem->valueint;

	subitem = cJSON_GetObjectItem(node, "omitted_posts");
	if(subitem) post->omitted_posts = subitem->valueint;

	subitem = cJSON_GetObjectItem(node, "omitted_images");
	if(subitem) post->omitted_images = subitem->valueint;

	subitem = cJSON_GetObjectItem(node, "replies");
	if(subitem) post->replies = subitem->valueint;

	subitem = cJSON_GetObjectItem(node, "images");
	if(subitem) post->images = subitem->valueint;

	subitem = cJSON_GetObjectItem(node, "bumplimit");
	if(subitem) post->bumplimit = subitem->valueint;

	subitem = cJSON_GetObjectItem(node, "imagelimit");
	if(subitem) post->imagelimit = subitem->valueint;

	subitem = cJSON_GetObjectItem(node, "last_modified");
	if(subitem) post->last_modified = subitem->valueint;

	subitem = cJSON_GetObjectItem(node, "tag");
	if(subitem) string_set(&post->tag, subitem->valuestring);

	subitem = cJSON_GetObjectItem(node, "semantic_url");
	if(subitem) string_set(&post->semantic_url, subitem->valuestring);

	subitem = cJSON_GetObjectItem(node, "since4pass");
	if(subitem) post->since4pass = subitem->valueint;

	return(post);
}


static void free_post(Post *post)
{
	string_clear(&post->now);
	string_clear(&post->name);
	string_clear(&post->trip);
	string_clear(&post->id);
	string_clear(&post->capcode);
	string_clear(&post->country);
	string_clear(&post->country_name);
	string_clear(&post->sub);
	string_clear(&post->com);
	string_clear(&post->filename);
	string_clear(&post->ext);
	string_clear(&post->md5);
	string_clear(&post->tag);
	string_clear(&post->semantic_url);
	free(post);
}

static int get_html(const char *board, const uint64_t id)
{
	FILE *file;
	String response = STRING_NULL;
	String url = STRING_NULL;
	String filename = STRING_NULL;

	if(string_setf(&url, endpoints[URLF_THREADS], board, id) == STRING_ERR) return(-1);
	if(string_setf(&filename, "%d.htm", id) == STRING_ERR)
	{
		string_clear(&url);
		return(-1);
	}

	if(http_get(&response, url.c_str) < 0)
	{
		string_clear(&url);
		string_clear(&filename);
		return(-1);
	}

	/* work on the html */

	for(;;)
	{
		if(string_replace(&response, "//i.4cdn.org/b/", "") == STRING_ERR) break;
	}

	for(;;)
	{
		if(string_replace(&response, "//s.4cdn.org", "..") == STRING_ERR) break;
	}

	/* write html to file */

	file = fopen(filename.c_str, "wt");
	if(!file) goto abort;

	fputs(response.c_str, file);

	fclose(file);

	abort:
		string_clear(&filename);
		string_clear(&url);
		string_clear(&response);

	return(0);
}

List *get_threads(const char *board)
{
	List *threadlist;
	String response = STRING_NULL;
	String url = STRING_NULL;
	cJSON *root;
	unsigned pages, i;

	/* create URL and perform get request */

	if(string_setf(&url, endpoints[URL_THREADS], board) == STRING_ERR) return(0);

	if(http_get(&response, url.c_str) < 0)
	{
		string_clear(&url);
		return(0);
	}

	string_clear(&url);

	/* work on the response */

	threadlist = malloc(sizeof(List));
	*threadlist = LIST_NULL;

	root = cJSON_Parse(response.c_str);

		/* go through pages */

		pages = cJSON_GetArraySize(root);

		for(i = 0; i < pages; i++)
		{
			cJSON *page, *threads;
			unsigned num_threads, c;

			page = cJSON_GetArrayItem(root, i);

			/* get the threads within a page */

			threads = cJSON_GetObjectItem(page, "threads");
			num_threads = cJSON_GetArraySize(threads);

				/* go through threads */

				for(c = 0; c < num_threads; c++)
				{
					cJSON *node;
					Thread *thread;

					node = cJSON_GetArrayItem(threads, c);
					thread = parse_thread(node);
					if(thread) list_add_last(threadlist, thread);
				}
		}

	cJSON_Delete(root);

	string_clear(&response);

	return(threadlist);
}

void free_threads(List *threads)
{
	while(!list_isempty(threads))
	{
		free_thread((Thread*)threads->last->obj);
		list_del_last(threads);
	}

	free(threads);
}

List *get_catalog(const char *board)
{
	List *catalog;
	String response = STRING_NULL;
	String url = STRING_NULL;
	cJSON *root;
	unsigned pages, i;

	/* create URL and perform get request */

	if(string_setf(&url, endpoints[URL_CATALOG], board) == STRING_ERR) return(0);

	if(http_get(&response, url.c_str) < 0)
	{
		string_clear(&url);
		return(0);
	}

	string_clear(&url);

	/* work on the response */

	catalog = malloc(sizeof(List));
	*catalog = LIST_NULL;

	root = cJSON_Parse(response.c_str);

		/* go through pages */

		pages = cJSON_GetArraySize(root);

		for(i = 0; i < pages; i++)
		{
			cJSON *page, *threads;
			unsigned num_threads, c;

			page = cJSON_GetArrayItem(root, i);

			/* get the threads within a page */

			threads = cJSON_GetObjectItem(page, "threads");
			num_threads = cJSON_GetArraySize(threads);

				/* go through threads */

				for(c = 0; c < num_threads; c++)
				{
					cJSON *node;
					Post *info;

					node = cJSON_GetArrayItem(threads, c);
					info = parse_post(node);
					if(info) list_add_last(catalog, info);
				}
		}

	cJSON_Delete(root);
	string_clear(&response);

	return(catalog);
}

void free_catalog(List *catalog)
{
	while(!list_isempty(catalog))
	{
		free_post((Post*)catalog->last->obj);
		list_del_last(catalog);
	}

	free(catalog);
}

List* get_replies(const char *board, const uint64_t id)
{
	List *replylist;
	String response = STRING_NULL;
	String url = STRING_NULL;
	cJSON *root, *posts;
	unsigned num_replies, i;

	/* create URL and perform get request */

	if(string_setf(&url, endpoints[URL_THREAD], board, id) == STRING_ERR) return(0);

	if(http_get(&response, url.c_str) < 0)
	{
		string_clear(&url);
		return(0);
	}

	/* work on the response */

	replylist = malloc(sizeof(List));
	*replylist = LIST_NULL;

	root = cJSON_Parse(response.c_str);
	posts = cJSON_GetObjectItem(root, "posts");

		/* go through replies */

		num_replies = cJSON_GetArraySize(posts);

		for(i = 0; i < num_replies; i++)
		{
			cJSON *post;
			Post *reply;

			post = cJSON_GetArrayItem(posts, i);

			reply = parse_post(post);
			if(reply) list_add_last(replylist, reply);
		}

	string_clear(&url);
	string_clear(&response);

	return(replylist);
}

int get_image(const char *board, const uint64_t tim, const char *ext)
{
	String url = STRING_NULL;
	String filename = STRING_NULL;

	if(string_setf(&url, endpoints[URL_IMAGES], board, tim, ext) == STRING_ERR) return(-1);
	if(string_setf(&filename, "%lu%s", tim, ext) == STRING_ERR)
	{
		string_clear(&url);
		return(-1);
	}

	/* If the file does already exist we can quit */
	if(access(filename.c_str, F_OK) == 0)
	{
		string_clear(&url);
		string_clear(&filename);
		return(0);
	}

	if(http_download(filename.c_str, url.c_str) < 0)
	{
		string_clear(&url);
		string_clear(&filename);
		return(-1);
	}

	string_clear(&url);
	string_clear(&filename);

	if(string_setf(&url, endpoints[URL_THUMBNAILS], board, tim) == STRING_ERR) return(-1);
	if(string_setf(&filename, "%lus%s", tim, ".jpg") == STRING_ERR)
	{
		string_clear(&url);
		return(-1);
	}

	/* If the file does already exist we can quit */
	if(access(filename.c_str, F_OK) == 0)
	{
		string_clear(&url);
		string_clear(&filename);
		return(0);
	}

	if(http_download(filename.c_str, url.c_str) < 0)
	{
		string_clear(&url);
		string_clear(&filename);
		return(-1);
	}


	string_clear(&url);
	string_clear(&filename);

	return(0);
}

int download_thread(const char *board, const uint64_t id)
{
	String directory = STRING_NULL;
	List *replies;
	List_node *node;

	string_setf(&directory, "./%d", id);

	/* If dir doesn't exist */
	if(chdir(directory.c_str) == -1)
	{
		mkdir(directory.c_str, 0777);
		chdir(directory.c_str);
	}

	get_html(board, id);

	replies = get_replies(board, id);
	if(!replies) goto abort;

	list_iterate(replies, node)
	{
		Post *reply;
		reply = (Post*)node->obj;

		if(reply->tim)
		{
			printf("downloading image [%lu] [%s]\n", reply->tim, reply->ext.c_str);
			get_image(board, reply->tim, reply->ext.c_str);
		}
	}

	free_replies(replies);

	abort:
	string_clear(&directory);
	chdir("..");

	return(0);
}

int archive_threads(const char *board)
{
	List *threads, *new_threads;
	List_node *old_node, *new_node;

	threads = get_threads(board);
	if(!threads) return(-1);

	for(;;)
	{
		sleep(10);

		new_threads = get_threads(board);
		if(!new_threads)
		{
			free_threads(threads);
			return(-1);
		}

		list_iterate(threads, old_node)
		{
			Thread *t_old;
			t_old = (Thread*)old_node->obj;

			list_iterate(new_threads, new_node)
			{
				Thread *t_new;
				t_new = (Thread*)new_node->obj;

				/* old and new threads match */
				if(t_old->no == t_new->no)
				{
					if(t_new->last_modified > t_old->last_modified)
					{
						printf("Updating archive of thread [%lu]\n", t_new->no);
						download_thread(board, t_new->no);
					}

					break;
				}
			}
		}

		free_threads(threads);
		threads = new_threads;
		new_threads = 0;
	}
}

Post *find_post(List *posts, const uint64_t no)
{
	List_node *node;

	list_iterate(posts, node)
	{
		Post *postptr;

		postptr = (Post*)node->obj;

		if(postptr->no == no) return(postptr);
	}

	return(0);
}

Post *search_post(List *posts, const char *needle)
{
	List_node *node;

	list_iterate(posts, node)
	{
		Post *postptr;

		postptr = (Post*)node->obj;

		if(strstr(postptr->com.c_str, needle) != 0) return(postptr); /* There might be more hits though */
	}

	return(0);
}