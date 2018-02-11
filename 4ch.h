/* 4chan read-only api in ANSI C
 * public domain
 ****************
 * currently *nix only (no winblows)
 * although easy modification would allow compilation
 * on win systems
 */

#ifndef FOURCH_H
#define FOURCH_H

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include "cJSON/cJSON.c"
#include "http/http.h"
#include "list/list.h"
#include "dstring/dstring.h"

#ifndef bool
	typedef uint8_t bool;
#endif

typedef struct
{
	uint64_t no;			/* Thread id */
	uint64_t last_modified;		/* Time of modification */
} Thread;

typedef struct
{
	uint64_t no;			/* Post number */
	uint64_t resto;			/* Reply to (0 == OP */
	bool sticky;			/* Stickied thread? */
	bool closed;			/* Closed thread? */
	bool archived;			/* Archived thread? */
	uint64_t archived_on;		/* Time when archived */
	String now;			/* Date and time */
	uint64_t time;			/* UNIX timestamp */
	String name;			/* Name of poster */
	String trip;			/* Tripcode */
	String id;			/* ID (Mod, Admin, etc) */
	String capcode;			/* Capcode (mod, admin, etc) */
	String country;			/* Country code */
	String country_name;		/* Country name */
	String sub;			/* Subject */
	String com;			/* Comment */
	uint64_t tim;			/* Renamed filename */
	String filename;		/* Original filename */
	String ext;			/* File extension */
	uint32_t fsize;			/* File size */
	String md5;			/* File MD5 */
	uint16_t w;			/* Image width */
	uint16_t h;			/* Image height */
	uint8_t tn_w;			/* Thumbnail width */
	uint8_t tn_h;			/* Thumbnail height */
	bool filedeleted;		/* File deleted? */
	bool spoiler;			/* Spoiler image? */
	uint8_t custom_spoiler;		/* Custom spoilers? */
	uint16_t omitted_posts;		/* # replies omitted */
	uint16_t omitted_images;	/* # images omitted */
	uint32_t replies;		/* # replies total */
	uint32_t images;		/* # images total */
	bool bumplimit;			/* Bump limit met? */
	bool imagelimit;		/* Image limit met? */
	/* TODO: capcode_replies */
	uint64_t last_modified;		/* Time when last modified */
	String tag;			/* Thread tag */
	String semantic_url;		/* Thread URL slug */
	uint16_t since4pass;		/* Year 4chan Pass bought */
} Post;

static const char *endpoints[] =
{
	/* JSON-API */
	"http://a.4cdn.org/%s/thread/%lu.json",		/* Thread (board, threadnumber) */
	"http://a.4cdn.org/%s/%u.json",			/* Page (board, pagenumber) */
	"http://a.4cdn.org/%s/catalog.json",		/* Catalog (board) */
	"http://a.4cdn.org/%s/threads.json",		/* Threads (board) */
	"http://a.4cdn.org/%s/archive.json",		/* Archive (board) */
	"http://a.4cdn.org/boards.json",		/* Boards */
	"http://i.4cdn.org/%s/%lu%s",			/* Images (board, tim, ext) */
	"http://t.4cdn.org/%s/%lus.jpg",		/* Thumbnails (board, tim) */

	/* FRONTEND */
	"http://boards.4chan.org/%s/",			/* Boards (board) */
	"http://boards.4chan.org/%s/%u",		/* Indexes (board, page) */
	"http://boards.4chan.org/%s/thread/%lu",	/* Threads (board, resto) */
	"http://boards.4chan.org/%s/thread/%lu#p%lu"	/* Replies (board, resto, no) */
};

enum
{
	URL_THREAD = 0,
	URL_PAGE,
	URL_CATALOG,
	URL_THREADS,
	URL_ARCHIVE,
	URL_BOARDS,
	URL_IMAGES,
	URL_THUMBNAILS,
	URLF_BOARDS,
	URLF_INDEXES,
	URLF_THREADS,
	URLF_REPLIES
};

int init_4ch();
void exit_4ch();
List *get_threads(const char *board);
void free_threads(List *threads);
List *get_catalog(const char *board);
void free_catalog(List *catalog);
List *get_replies(const char *board, const uint64_t no);
#define free_replies(c) free_catalog(c)
int get_image(const char *board, const uint64_t tim, const char *ext);
int download_thread(const char *board, const uint64_t no);
Post *find_post(List *posts, const uint64_t no); /* Applies to replies and threads in post format */
Post *search_post(List *posts, const char *needle); /* Applies to replies and threads in post format */

#include "4ch.c"

#endif /* FOURCH_H */