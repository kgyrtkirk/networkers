/***********************************************************************


	File:	bbmain.c
	Rev:	a-1
	Date:	11/24/2000

	Copyright (c) 1995, 2001 by David M. Warme

************************************************************************

	The main routine for the branch-and-cut.  (Actually the name
	"bc" is taken so I use "bb" for branch-and-bound.)  It takes
	a file of FSTs and finds the Steiner minimal tree.

************************************************************************

	Modification Log:

	a-1:	11/24/2000	warme
		: Split the main() function off into this new file
		:  so that other programs can call branch-and-cut().
		: Added new switches: -a, -B, -z.

************************************************************************/

#include "bb.h"
#include "bbsubs.h"
#include "config.h"
#include "constrnt.h"
#include "genps.h"
#include "p1io.h"
#include "steiner.h"


/*
 * Global Routines
 */

	static
	int
get_d (void)

{
int		i, n;

	n = scanf (" %d", &i);
	if (n NE 1) {
		fprintf (stderr, "Unexpected EOF!\n");
		exit (1);
	}

	return (i);
}

int			main (int, char **);



/*
 * External References
 */

extern int		Branch_Var_Policy;
extern int		Check_Branch_Vars_Thoroughly;
extern bool		Check_Root_Constraints;
extern bool		Choose_Branch_Vars_Carefully;
extern double		Initial_Upper_Bound;
extern bool		Print_Root_LP;
extern int		Target_Pool_Non_Zeros;

#ifdef CPLEX
extern int		min_cplex_rows;
extern int		min_cplex_nzs;
#endif

#ifdef LPSOLVE
extern bool		Use_Perturbations;
extern bool		Use_Scaling;
#endif


	static
	void
efst_print(

struct full_set *	fsp,		/* IN - FST to draw */
struct cinfo *		cip		/* IN - compatibility info */
)
{
int			i;
int			j;
int			nt;
struct point *		p1;
struct point *		p2;
struct pset *		terms;
struct pset *		steins;
struct edge *		ep;
char			buf1 [32];
char			buf2 [32];

	terms = fsp -> terminals;
	steins = fsp -> steiners;

	nt = terms -> n;

// 	printf("nv:%d\n",cip -> num_verts);
// 	printf("nv:%d\n",steins->n);
// // 	printf("nv:%d\n",steins->n);
// 	for (i = 0; i < cip -> num_verts; i++, ep++) {
// 		printf("\% @T	%f	%f\n",terms -> a [i].x,terms -> a [i].y);
// 	}
// 	for (i = 0; i < steins->n; i++, ep++) {
// 		printf("\% @S	%f	%f\n",steins -> a [i].x,steins-> a [i].y);
// 	}
	
	
	ep = fsp -> edges;
// 		printf (" XXXXXXXXXXxnE	%d\n", fsp -> nedges);
	for (i = 0; i < fsp -> nedges; i++, ep++) {
// 		printf(".\n");
		j = ep -> p1;
		printf ("% @E	%d	--	%d\n",  ep -> p1, ep -> p2);
// 		printf ("	%d	--	%d\n",  ep -> p1, ep -> p2);

		if (j < nt) {
			p1 = &(terms -> a [j]);
			(void) printf ("\t%d T", p1 -> pnum);
		}
		else {
			p1 = &(steins -> a [j - nt]);
			coord_to_string (buf1, p1 -> x, &(cip -> scale));
			coord_to_string (buf2, p1 -> y, &(cip -> scale));
			(void) printf ("	>%d\n", j,buf1, buf2);
		}
		p2=p1;
		j = ep -> p2;
		if (j < nt) {
			p1 = &(terms -> a [j]);
			(void) printf ("\t%d T\tS\n", p1 -> pnum);
		}
		else {
			p1 = &(steins -> a [j - nt]);
			coord_to_string (buf1, p1 -> x, &(cip -> scale));
			coord_to_string (buf2, p1 -> y, &(cip -> scale));
			(void) printf ("	>%d\n", j,buf1, buf2);
		}
		printf ("% @@E	%f	%f	%f	%f\n",  p1->x,p1->y,p2->x,p2->y);
	}
}

void	print_dot(bitmap_t *		fset_mask,struct	cinfo*cip)
{
	int	i;
	int	n;
	struct full_set *	fsp;

	
	n = cip -> num_edges;
	
	if ((cip -> pts NE NULL) AND (cip -> full_trees NE NULL)) {
		/* Draw the FSTs with postscript. */
// 		begin_plot (plot_size);
		(void) printf ("\tPlot_Terminals\n");

		for (i = 0; i < n; i++) {
			if (NOT BITON (fset_mask, i)) continue;
			printf("TREE\n");
			
			fsp = cip -> full_trees [i];
			efst_print(fsp, cip);
// 			return;
// 			fst_comment (fsp);
		}

// 		end_plot (title);
	}
	
	
// 	printf("x	%d\n",ci->);
}


/*
 * Local Routines
 */

static int		atoi_suf (const char *);
static void		decode_params (int, char **);
static void		usage (void);


/*
 * Local Variables
 */


	static
	void
read_phase_tx_data (

struct cinfo *	cip,		/* OUT - compatibility info. */
int		version		/* IN - version number. */
)
{
int			i;
int			j;
int			k;
char			c;
int			nverts;
int			nedges;
int			kmasks;
int			nmasks;
int			scale_factor;
int			total_edge_cardinality;
int			nt;
char *			s;
struct numlist *	line;
struct numlist *	p;
struct numlist **	hookp;
struct numlist **	prev_hookp;
struct numlist *	costs;
struct numlist **	cost_hookp;
int *			ip1;
int *			ip2;

	nverts = get_d ();
	nedges = get_d ();

	kmasks = BMAP_ELTS (nverts);
	nmasks = BMAP_ELTS (nedges);

	cip -> num_verts	= nverts;
	cip -> num_edges	= nedges;
	cip -> num_vert_masks	= kmasks;
	cip -> num_edge_masks	= nmasks;

	cip -> edge		= NEWA (nedges + 1, int *);
	cip -> edge_size	= NEWA (nedges, int);
	cip -> cost		= NEWA (nedges, dist_t);
	cip -> tflag		= NEWA (nverts, bool);
	cip -> metric		= PURE_GRAPH;

	cip -> initial_vert_mask = NEWA (kmasks, bitmap_t);
	cip -> initial_edge_mask = NEWA (nmasks, bitmap_t);
	cip -> required_edges	 = NEWA (nmasks, bitmap_t);

	memset (cip -> initial_vert_mask, 0, kmasks * sizeof (bitmap_t));
	memset (cip -> initial_edge_mask, 0, nmasks * sizeof (bitmap_t));
	memset (cip -> required_edges,	  0, nmasks * sizeof (bitmap_t));

	for (i = 0; i < nverts; i++) {
		SETBIT (cip -> initial_vert_mask, i);
	}
	for (i = 0; i < nedges; i++) {
		SETBIT (cip -> initial_edge_mask, i);
	}

	costs = NULL;
	cost_hookp = &costs;

	total_edge_cardinality = 0;

	for (i = 0; i < nedges; i++) {
		line = parse_line_of_numbers (stdin);
		if (line EQ NULL) {
			fprintf (stderr, "Unexpected EOF!\n");
			exit (1);
		}

		/* Count the numbers and detach the last one */
		j = 0;
		prev_hookp = NULL;
		hookp = &line;
		for (;;) {
			p = *hookp;
			if (p EQ NULL) break;
			prev_hookp = hookp;
			hookp = &(p -> next);
			++j;
		}
		if (j < 3) {
			fprintf (stderr,
				 "Hyperedge with less than 2 vertices!\n");
			exit (1);
		}

		/* Transfer last number onto cost list. */
		p = *prev_hookp;
		*prev_hookp = NULL;
		--j;
		cip -> edge_size [i] = j;
		total_edge_cardinality += j;

		*cost_hookp = p;
		cost_hookp = &(p -> next);

		ip1 = NEWA (j, int);
		cip -> edge [i] = ip1;

		/* Verify remaining numbers are integers and convert. */
		j = 0;
		for (p = line; p NE NULL; p = p -> next) {
			if (p -> expon < 0) goto nonint;
			if (p -> expon > 0) {
				do {
					p -> mantissa *= 10.0;
				} while (--(p -> expon) > 0);
			}
			if ((p -> mantissa < 1) OR
			    (nverts < p -> mantissa) OR
			    (p -> mantissa NE floor (p -> mantissa))) {
				fprintf (stderr,
					 "Vertex number %g out of range.\n",
					 p -> mantissa);
				exit (1);
			}
			*ip1++ = p -> mantissa - 1;
		}
		if (cip -> edge [i] + cip -> edge_size [i] NE ip1) {
			fatal ("read_version_0: Bug 1.");
		}
	}

	/* Now convert the hyperedge costs. */
	scale_factor = compute_scaling_factor (costs);

	set_scale_info (&(cip -> scale), scale_factor);

	i = 0;
	while (costs NE NULL) {
		p = costs;
		costs = p -> next;
		p -> next = NULL;
		cip -> cost [i++] = read_numlist (p, &(cip -> scale));
	}
	if (i NE nedges) {
		fatal ("read_version_0: Bug 2.");
	}
	printf("xxx -> %d\n",nverts);
	/* Read in the list of terminal vertices... */
	if (scanf (" %d", &nt) NE 1) {
		/* EOF -- assume all vertices are terminals. */
		for (i = 0; i < nverts; i++) {
			cip -> tflag [i] = TRUE;
		}
	}
	else {
		/* All are Steiner vertices unless listed! */
		for (i = 0; i < nverts; i++) {
			cip -> tflag [i] = FALSE;
		}
		for (i = 0; i < nt; i++) {
			j = get_d ();
			if ((j < 1) OR (nverts < j)) {
				fatal ("read_version_0: Bug 3.");
			}
			cip -> tflag [j - 1] = TRUE;
		}
	}

	/* Copy hyperedges into contiguous form. */
	ip1 = NEWA (total_edge_cardinality, int);
	for (i = 0; i < nedges; i++) {
		ip2 = cip -> edge [i];
		cip -> edge [i] = ip1;
		k = cip -> edge_size [i];
		for (j = 0; j < k; j++) {
			*ip1++ = ip2 [j];
		}
		free ((char *) ip2);
	}
	cip -> edge [i] = ip1;

	init_term_trees (cip);
	cip -> inc_edges = compute_basic_incompat (cip);
	return;

	/* Error exit. */
nonint:
	fprintf (stderr, "Expected integer!\n");
	exit (1);
}



static int32u		cpu_time_limit;
static char *		me;

/*
 * The main routine for the "bb" program.  It takes the output from
 * the "prep" program (phase 1 of our Steiner tree program) and uses
 * a branch-and-cut to find the Steiner minimal tree.
 */

	int
main (

int		argc,
char **		argv
)
{
int			i;
int			j;
int			fpsave;
bitmap_t *		smt_mask;
dist_t			length;
char *			descr;
struct pset *		pts;
char *			cp1;
char *			cp2;
char			c;
int *			edge_count;
int			smt_edge_count;
int			total_edge_count;
int			edge10;
int			max_edge_size;
int			nt;
struct bbinfo *		bbip;
struct bbstats *	statp;
double			gap;
double			redmst;
struct cinfo		cinfo;
char			buf1 [32];
char			buf2 [32];
char			buf3 [32];
char			buf4 [32];
char			title [256];

	fpsave = set_floating_point_double_precision ();

	setbuf (stdout, NULL);

	decode_params (argc, argv);

	init_tables ();

	read_phase_1_data (&cinfo);
// 	read_phase_tx_data (&cinfo,0);

	/* Enable CPU time limitation, if any. */
	start_limiting_cpu_time (cpu_time_limit);

	startup_lp_solver ();

	convert_cpu_time (cinfo.p1time, buf1);
	printf (" %% Phase 1: %s seconds\n", buf1);

	define_Plot_Terminals (cinfo.pts, &cinfo.scale);

	bbip = create_bbinfo (&cinfo);

	/* Do the branch-and-cut... */
	length = branch_and_cut (bbip);

	smt_mask = bbip -> smt;
	statp = bbip -> statp;

	/* Tally various statistics about the solution.		*/

	edge_count = NEWA (cinfo.num_verts + 1, int);
	total_edge_count = 0;
	smt_edge_count = 0;
	max_edge_size = 0;
	for (i = 0; i <= cinfo.num_verts; i++) {
		edge_count [i] = 0;
	}

	for (i = 0; i < cinfo.num_edges; i++) {
		if (NOT BITON (smt_mask, i)) continue;

		nt = cinfo.edge_size [i];

		++(edge_count [nt]);
		total_edge_count += nt;
		if (nt > max_edge_size) {
			max_edge_size = nt;
		}
		++smt_edge_count;
	}

	if (cinfo.full_trees NE NULL) {
		/* Print out a certificate of the solution.  This	*/
		/* consists of the coordinates of each of the Steiner	*/
		/* points.						*/

		printf ("\n %% Certificate of solution:\n");
		for (i = 0; i < cinfo.num_edges; i++) {
			if (NOT BITON (smt_mask, i)) continue;

			pts = cinfo.full_trees [i] -> steiners;
			for (j = 0; j < pts -> n; j++) {
				coord_to_string (buf1,
						 pts -> a [j].x,
						 &cinfo.scale);
				coord_to_string (buf2,
						 pts -> a [j].y,
						 &cinfo.scale);
				printf (" %% @C	%s	%s\n", buf1, buf2);
			}
		}
	}

	convert_cpu_time (statp -> p1time + statp -> p2time, buf1);
	dist_to_string (buf2, length, &cinfo.scale);

	descr = "Steiner Minimal Tree";
	if ((cinfo.description NE NULL) AND (cinfo.description [0] NE '\0')) {
		descr = cinfo.description;
	}
	sprintf (title,
		 "%s:  %lu points,  length = %s,  %s seconds",
		 descr, cinfo.num_verts, buf2, buf1);

// // 	overlay_plot_subset (title, smt_mask, &cinfo, BIG_PLOT);
	printf("---\n");
	print_dot(smt_mask,&cinfo);

	/* Print out statistics for this run. */
	gap = 100.0 * (statp -> z - statp -> root_z) / statp -> z;
	convert_cpu_time (statp -> p1time, buf3);
	convert_cpu_time (statp -> p2time, buf4);

	/* Problem summary... */
// 	printf ("%% @0 %s\n",
// 		cinfo.description NE NULL ? cinfo.description : "");
// 	printf ("%% N M Nodes LPs P1CPU P2CPU TotCPU\n");
// 	printf ("%% @1 %d %d %d %d %s %s %s\n",
// 		statp -> n,
// 		statp -> m,
// 		statp -> num_nodes,
// 		statp -> num_lps,
// 		buf3, buf4, buf1);

	/* Solution and root node statistics... */
	if (statp -> root_opt) {
		sprintf (buf3, "%18.6f",
			 UNSCALE (statp -> root_z, &cinfo.scale));
	}
	else {
		sprintf (buf3, "(%18.6f)",
			 UNSCALE (statp -> root_z, &cinfo.scale));
	}
	cp1 = &buf3 [0];
	cp2 = &buf3 [0];
	for (;;) {		/* delete spaces... */
		c = *cp2++;
		if ((c NE ' ') AND ((*cp1++ = c) EQ '\0')) break;
	}

	convert_cpu_time (statp -> root_time, buf4);
	if (cinfo.mst_length > 0) {
		redmst = 100.0 * (cinfo.mst_length - length)
				/ cinfo.mst_length;
	}
	else {
		redmst = 0.0;
	}
// 	printf ("%% Z RootZ %%Gap RootLPs RootCPU RedMST\n");
// 	printf ("%% @2 %s %s %7.5f %d %s %.4f\n",
// 		buf2,
// 		buf3,
// 		gap,
// 		statp -> root_lps,
// 		buf4,
// 		redmst);
// 
// 	/* Initial constraint pool statistics... */
// 	printf ("%% InitPRows InitPNZ InitLPRows InitLPNZ\n");
// 	printf ("%% @3 %d %d %d %d\n",
// 		statp -> cs_init.num_prows,
// 		statp -> cs_init.num_pnz,
// 		statp -> cs_init.num_lprows,
// 		statp -> cs_init.num_lpnz);
// 
// 	/* Root constraint pool statistics... */
// 	printf ("%% RootPRows RootPNZ RootLPRows RootLPNZ\n");
// 	printf ("%% @4 %d %d %d %d\n",
// 		statp -> cs_root.num_prows,
// 		statp -> cs_root.num_pnz,
// 		statp -> cs_root.num_lprows,
// 		statp -> cs_root.num_lpnz);
// 
// 	/* Final constraint statistics... */
// 	printf ("%% FinalPRows FinalPNZ FinalLPRows FinalLPNZ\n");
// 	printf ("%% @5 %d %d %d %d\n",
// 		statp -> cs_final.num_prows,
// 		statp -> cs_final.num_pnz,
// 		statp -> cs_final.num_lprows,
// 		statp -> cs_final.num_lpnz);

	/* Statistics on the SMT: number of FSTs, size and distribution. */

	edge10 = 0;
// 	printf ("%% SMTFSTs SMTAvgFSTSz SMTMaxFSTSz #2FSTs #3FSTs ... #10FSTS #>10FSTs\n");
// 	printf ("%% @6 %d %f %d",
// 		smt_edge_count,
// 		((double) total_edge_count) / ((double) smt_edge_count),
// 		max_edge_size);
	for (i = 2; i <= 10; i++) {
		j = (i <= cinfo.num_verts) ? edge_count [i] : 0;
		edge10 += j;
// 		printf (" %d", j);
	}
// 	printf (" %d\n", smt_edge_count - edge10);

	destroy_bbinfo (bbip);

	shutdown_lp_solver ();

	free_phase_1_data (&cinfo);

	restore_floating_point_precision (fpsave);

	exit (0);
}

/*
 * This routine decodes the various command-line arguments.
 */

	static
	void
decode_params (

int		argc,
char **		argv
)
{
char *		ap;
char		c;
struct slist *	p;

	--argc;
	me = *argv++;

	printf (" %% %s\n", me);
	printf (" %% Args:\n");

	while (argc > 0) {
		ap = *argv++;
		if (*ap NE '-') {
			usage ();
		}
		printf (" %%	%s\n", ap);
		++ap;
		while ((c = *ap++) NE '\0') {
			switch (c) {
			case '2':
				Seed_Pool_With_2SECs = FALSE;
				break;

#ifdef CPLEX
			case 'a':
				if ((*ap NE '\0') OR (argc <= 1)) {
					usage ();
				}
				ap = *argv++;
				printf (" %%	%s\n", ap);
				--argc;
				min_cplex_rows = atoi_suf (ap);
				ap = *argv++;
				printf (" %%	%s\n", ap);
				--argc;
				min_cplex_nzs = atoi_suf (ap);
				ap = "";
				break;
#endif

			case 'b':
				Choose_Branch_Vars_Carefully = FALSE;
				break;

			case 'B':
				if (*ap EQ '\0') {
					if (argc <= 0) {
						usage ();
					}
					ap = *argv++;
					printf (" %%	%s\n", ap);
					--argc;
				}
				Branch_Var_Policy = atoi_suf (ap);
				if ((Branch_Var_Policy < 0) OR
				    (Branch_Var_Policy > 2)) {
					usage ();
				}
				ap = "";
				break;

			case 'l':
				if (*ap EQ '\0') {
					if (argc <= 0) {
						usage ();
					}
					ap = *argv++;
					printf (" %%	%s\n", ap);
					--argc;
				}
				if (NOT decode_cpu_time_limit (ap, &cpu_time_limit)) {
					usage ();
				}
				ap = "";
				break;

#ifdef LPSOLVE
			case 'p':
				Use_Perturbations = TRUE;
				break;
#endif

			case 'r':
				Print_Root_LP = TRUE;
				break;

			case 'R':
				Check_Root_Constraints = TRUE;
				break;

#ifdef LPSOLVE
			case 's':
				Use_Scaling = TRUE;
				break;
#endif

			case 'T':
				if (*ap EQ '\0') {
					if (argc <= 0) {
						usage ();
					}
					ap = *argv++;
					printf (" %%	%s\n", ap);
					--argc;
				}
				Check_Branch_Vars_Thoroughly = atoi_suf (ap);
				if (Check_Branch_Vars_Thoroughly < 1) {
					usage ();
				}
				ap = "";
				break;

			case 'u':
				if (*ap EQ '\0') {
					if (argc <= 0) {
						usage ();
					}
					ap = *argv++;
					printf (" %%	%s\n", ap);
					--argc;
				}
				Initial_Upper_Bound = atof (ap);
				ap = "";
				break;

			case 'z':
				if (*ap EQ '\0') {
					if (argc <= 0) {
						usage ();
					}
					ap = *argv++;
					printf (" %%	%s\n", ap);
					--argc;
				}
				Target_Pool_Non_Zeros = atoi_suf (ap);
				ap = "";
				break;

			default:
				usage ();
				break;
			}
		}
		--argc;
	}
}

/*
 * This routine prints out the proper usage and exits.
 */

static char *	arg_doc [] = {
	"",
	"\t-2\tOmit all 2-terminal SECs from the initial",
	"\t\t constraint pool.",
#ifdef CPLEX
	"\t-a M N\tForce CPLEX allocation to be at least M",
	"\t\t rows and N non-zeros.",
#endif
	"\t-b\tDisable \"strong branching\", which chooses",
	"\t\tbranching variables very carefully.",
	"\t-B N\tSet branch variable selection policy.",
	"\t\t N=0: naive max of mins,",
	"\t\t N=1: smarter lexicographic max of mins (default),",
	"\t\t N=2: product of improvements.",
	"\t-l T\tTerminate run after T CPU time is expended.",
	"\t\t T can be in days, hours, minutes and/or seconds",
	"\t\t (as shown below).",
#ifdef LPSOLVE
	"\t-p\tUse perturbations when solving LP's.",
#endif
	"\t-r\tPrint root LP relaxation, if fractional.",
	"\t-R\tWhen optimal root LP relaxation is obtained,",
	"\t\tdetermine for each LP iteration the number of",
	"\t\tfinal constraints whose first violation occurred",
	"\t\tduring that iteration.",
#ifdef LPSOLVE
	"\t-s\tUse scaling when solving LP's.",
#endif
	"\t-T N\tSearch N times more thoroughly for strong",
	"\t\t branching variables.",
	"\t-u B\tSets the initial upper bound to B.",
	"\t-z N\tSets the target number of pool non-zeros to N.",
	"",
	"\tExample CPU times are:",
	"\t\t-l 3days2hours30minutes15seconds",
	"\t\t-l1000seconds -l1000 -l 2h30m",
	"",
	NULL
};

	static
	void
usage (void)

{
char **		pp;
char *		p;

	(void) fprintf (stderr,
			"\nUsage: %s"
			" [-2b"
#ifdef LPSOLVE
			"p"
#endif
			"rR"
#ifdef LPSOLVE
			"s"
#endif
			"]"
#ifdef CPLEX
			" [-a minNumRows minNumNonZeros]"
#endif
			" [-B branch_var_policy]"
			" [-l cpu-time-limit]"
			" [-T N]"
			" [-u upper-bound]"
			" [-z N] <phase-1-data-file\n",
			me);
	pp = &arg_doc [0];
	while ((p = *pp++) NE NULL) {
		(void) fprintf (stderr, "%s\n", p);
	}
	exit (1);
}

/*
 * Convert a decimal string to an integer, and permit various suffixes,
 * such as 'k' = 1000, 'K' = 1024, etc.
 */

	static
	int
atoi_suf (

const char *	s		/* IN - string to convert */
)
{
int		c;
int		sign;
int		num;

	do {
		c = *s++;
	} while (isspace (c));

	sign = 1;
	if (c EQ '-') {
		sign = -1;
	}

	num = 0;
	while ((c >= '0') AND (c <= '9')) {
		num = 10 * num + (c - '0');
		c = *s++;
	}
	switch (c) {
	case '\0':				break;
	case 'k':	num *= 1000;		break;
	case 'K':	num *= 1024;		break;
	case 'm':	num *= 1000000;		break;
	case 'M':	num *= (1024 * 1024);	break;
	default:
		fprintf (stderr, "%s: Unknown numeric suffix '%c'.\n",
			 me, c);
		exit (1);
	}

	return (sign * num);
}
