
#include <stdint.h>
#include <stdio.h>

#include "tlb.h"

#include "conf.h"

struct tlb_entry
{
  unsigned int page_number;
  int frame_number;             /* Invalide si négatif.  */
  bool readonly : 1;
};

static FILE *tlb_log = NULL;
static struct tlb_entry tlb_entries[TLB_NUM_ENTRIES]; 

static unsigned int tlb_hit_count = 0;
static unsigned int tlb_miss_count = 0;
static unsigned int tlb_mod_count = 0;

/* Initialise le TLB, et indique où envoyer le log des accès.  */
void tlb_init (FILE *log)
{
  for (int i = 0; i < TLB_NUM_ENTRIES; i++)
    tlb_entries[i].frame_number = -1;
  tlb_log = log;
}

/******************** ¡ NE RIEN CHANGER CI-DESSUS !  ******************/

// Lookup log counter for Least Recently Used (LRU) algorithm
static int tlb_log_lookup[TLB_NUM_ENTRIES];

// TODO: modifier si besoin
/* Recherche dans le TLB.
 * Renvoie le `frame_number`, si trouvé, ou un nombre négatif sinon.  */
static int tlb__lookup (unsigned int page_number, bool write)
{
    // if tlb has no log modifications ==> it is empty, return -1
    if (tlb_mod_count == 0) return -1;

    for (int i = 0; i < tlb_mod_count && i < TLB_NUM_ENTRIES; i++) {
        if (tlb_entries[i].page_number == page_number && tlb_entries[i].frame_number != -1 && !(write && tlb_entries[i].readonly)) {

            // counter for Least Recently Used (LRU) algorithm
            // For each tlb lookup log, if page is not in use decrement value, otherwise increment.
            // The page with minimal value should be replaced as least recently used when tlb memory is full.
            tlb_log_lookup[i]++;
            for (int pn = i + 1; pn < tlb_mod_count && pn < TLB_NUM_ENTRIES; pn++)
                tlb_log_lookup[pn]--;

            return tlb_entries[i].frame_number;

        } else if(!(write && tlb_entries[i].readonly))
            tlb_log_lookup[i]--;
    }

    return -1;
}

// TODO: modifier si besoin
/* Ajoute dans le TLB une entrée qui associe `frame_number` à
 * `page_number`.  */
static void tlb__add_entry (unsigned int page_number,
                            unsigned int frame_number, bool readonly)
{
    // If tlb has page number log, update it
    for (int i = 0; i < TLB_NUM_ENTRIES; i++) {
        if (tlb_entries[i].page_number == page_number) {
            tlb_entries[i].frame_number = (int) frame_number;
            tlb_entries[i].readonly = readonly;
            return;
        }
    }

    // If tlb is full
    if ( (tlb_mod_count - 1) >= TLB_NUM_ENTRIES) {
        int min = (int) tlb_log_lookup[0];
        int least_recently_used = 0;


        // Find Least Recently Used index
        for (int i = 1; i < TLB_NUM_ENTRIES; i++) {
            if (tlb_log_lookup[i] < min) {
                min = (int) tlb_log_lookup[i];
                least_recently_used = i;
            }
        }

        // Add page number
        tlb_entries[least_recently_used].page_number = page_number;
        tlb_entries[least_recently_used].frame_number = (int) frame_number;
        tlb_entries[least_recently_used].readonly = readonly;

        tlb_log_lookup[least_recently_used] = 0;

    } else {

        // Find first available index and add page number
        for (int i = 0; i < TLB_NUM_ENTRIES; i++) {
            if (tlb_entries[i].frame_number == -1) {
                tlb_entries[i].page_number = page_number;
                tlb_entries[i].frame_number = (int) frame_number;
                tlb_entries[i].readonly = readonly;
                break;
            }
        }
    }
}

/******************** ¡ NE RIEN CHANGER CI-DESSOUS !  ******************/

void tlb_add_entry (unsigned int page_number,
                    unsigned int frame_number, bool readonly)
{
  tlb_mod_count++;
  tlb__add_entry (page_number, frame_number, readonly);
}

int tlb_lookup (unsigned int page_number, bool write)
{
  int fn = tlb__lookup (page_number, write);
  (*(fn < 0 ? &tlb_miss_count : &tlb_hit_count))++;
  return fn;
}

/* Imprime un sommaires des accès.  */
void tlb_clean (void)
{
  fprintf (stdout, "TLB misses   : %3u\n", tlb_miss_count);
  fprintf (stdout, "TLB hits     : %3u\n", tlb_hit_count);
  fprintf (stdout, "TLB changes  : %3u\n", tlb_mod_count);
  fprintf (stdout, "TLB miss rate: %.1f%%\n",
           100 * tlb_miss_count
           /* Ajoute 0.01 pour éviter la division par 0.  */
           / (0.01 + tlb_hit_count + tlb_miss_count));
}
