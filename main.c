#include "4ch.h"

void print_post(const Post *post)
{
	printf(	"no: %lu\n"
		"resto: %lu\n"
		"sticky: %u\n"
		"closed: %u\n"
		"archived: %u\n"
		"archived_on: %lu\n"
		"now: %s\n"
		"time: %lu\n"
		"name: %s\n"
		"trip: %s\n"
		"id: %s\n"
		"capcode: %s\n"
		"country: %s\n"
		"country_name: %s\n"
		"sub: %s\n"
		"com: %s\n"
		"tim: %lu\n"
		"filename: %s\n"
		"ext: %s\n"
		"fsize: %u\n"
		"md5: %s\n"
		"w: %u\n"
		"h: %u\n"
		"tn_w: %u\n"
		"tn_h: %u\n"
		"filedeleted: %u\n"
		"spoiler: %u\n"
		"custom_spoiler: %u\n"
		"omitted_posts: %u\n"
		"omitted_images: %u\n"
		"replies: %u\n"
		"images: %u\n"
		"bumplimit: %u\n"
		"imagelimit: %u\n"
		"last_modified: %lu\n"
		"tag: %s\n"
		"semantic_url: %s\n"
		"since4pass: %u\n",
		post->no,
		post->resto,
		post->sticky,
		post->closed,
		post->archived,
		post->archived_on,
		post->now.c_str,
		post->time,
		post->name.c_str,
		post->trip.c_str,
		post->id.c_str,
		post->capcode.c_str,
		post->country.c_str,
		post->country_name.c_str,
		post->sub.c_str,
		post->com.c_str,
		post->tim,
		post->filename.c_str,
		post->ext.c_str,
		post->fsize,
		post->md5.c_str,
		post->w,
		post->h,
		post->tn_w,
		post->tn_h,
		post->filedeleted,
		post->spoiler,
		post->custom_spoiler,
		post->omitted_posts,
		post->omitted_images,
		post->replies,
		post->images,
		post->bumplimit,
		post->imagelimit,
		post->last_modified,
		post->tag.c_str,
		post->semantic_url.c_str,
		post->since4pass);
}

void ticker(const char *board)
{
	List *catalog;
	List *new_catalog;
	List_node *node;

	catalog = get_catalog(board);
	if(!catalog) return;

	for(;;)
	{
		sleep(2);
		puts("updating...");
		new_catalog = get_catalog(board);

		list_iterate(catalog, node)
		{
			List_node *node2;
			Post *p1;

			p1 = (Post*)node->obj;

			list_iterate(new_catalog, node2)
			{
				Post *p2;

				p2 = (Post*)node2->obj;

				if(p1->no == p2->no)
				{
					if(p1->last_modified < p2->last_modified)
					{
						printf("Activity in thread [%lu] [%s].\n", p1->no, p1->semantic_url.c_str);
					}

					break;
				}
			}
		}

		free_catalog(catalog);
		catalog = new_catalog;
	}

	free_catalog(catalog);
	free_threads(new_catalog);
}

void show_catalog(const char *board)
{
	List *catalog;
	List_node *node;

	catalog = get_catalog(board);
	if(!catalog) return;

	list_iterate(catalog, node)
	{
		Post *post = (Post*)node->obj;

		printf("Thread [%lu] [%s]\n", post->no, post->semantic_url.c_str);
	}

	free_catalog(catalog);
}

int main(int argc, char *argv[])
{
	if(argc < 3) return(0);

/*	ticker(argv[1]); */

/*	show_catalog(argv[1]); */

	if(chdir(argv[2]) == -1)
	{
		return(-1);
	}

	return(0);
}