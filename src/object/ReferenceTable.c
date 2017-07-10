#include "Object.h"
#include "ErrorHandling.h"

#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

static bool
is_prime (unsigned int x)
{
  double lim = sqrt ((double) x);
  for (int i = 2; i <= lim; i++)
    {
      if (x % i == 0)
	return false;
    }

  return true;
}

static unsigned int
next_prime (const unsigned int x)
{
  if (x < 3)
    return 3;

  unsigned int ret = x % 2 == 0 ? x + 1 : x + 2;
  while (!is_prime (ret))
    ret += 2;

  return ret;
}

struct Entry {
  const _Object *ref;
  unsigned long count;

  struct Entry *next;
};

static struct Entry *
entry_create (const _Object *ref)
{
  struct Entry *ret = malloc (sizeof (struct Entry));
  if (!ret)
    return NULL;

  ret->ref = ref;
  ret->count = 1;
  ret->next = NULL;

  return ret;
}

struct HashTable {
  unsigned int size;
  unsigned int entries;
  unsigned int functions_modifying;

  struct Entry **table;
};

struct HashTable *reference_table = NULL;

static struct HashTable *
hash_table_create (const unsigned int _size)
{
  const unsigned int size = is_prime (_size) ? _size : next_prime (_size);
  struct HashTable *ret = malloc (sizeof (struct HashTable));
  if (!ret)
    return NULL;

  ret->size = size;
  ret->entries = 0;
  ret->functions_modifying = 0;

  ret->table = calloc (size, sizeof (struct Entry *));
  if (!ret->table)
    {
      free (ret);
      return NULL;
    }

  return ret;
}

static void
hash_table_delete (struct HashTable *table)
{
  for (int i = 0; i < table->size; i++)
      {
        struct Entry *bin = table->table[i];
        while (bin)
  	{
  	  struct Entry *temp = bin->next;
  	  free (bin);
  	  bin = temp;
  	}
      }

  free (table->table);
  free (table);
}

static unsigned int
hash_table_get_index (const struct HashTable *table, const void *ref)
{
  unsigned int hash =
      (unsigned int) (unsigned long) ref;

  return hash % table->size;
}

static void
hash_table_set_count (struct HashTable *table, const void *_ref, unsigned int val)
{
  exit_if_not (table != NULL, "ReferenceTable.c:hash_table_add_ref needs a non null table.");
    table->functions_modifying ++;
    const _Object *ref = cast (_ref, Object);

    unsigned int index = hash_table_get_index (table, ref);
    struct Entry *bin = table->table[index];

    while (bin)
      {
	if (bin->ref == ref)
	  {
	    bin->count = val;
	  }

	bin = bin->next;
      }
}

static void
hash_table_add_ref (struct HashTable *table, const void *_ref)
{
  exit_if_not (table != NULL, "ReferenceTable.c:hash_table_add_ref needs a non null table.");
  table->functions_modifying ++;
  const _Object *ref = cast (_ref, Object);

  unsigned int index = hash_table_get_index (table, ref);
  struct Entry *bin = table->table[index];

  if (bin == NULL)
    {
      table->table[index] = entry_create (ref);
      table->entries ++;
      table->functions_modifying --;
      return;
    }
  else
    {
      while (bin->next)
	{
	  if (bin->ref == ref)
	    {
	      bin->count ++;
	      table->functions_modifying --;
	      return;
	    }

	  bin = bin->next;
	}

      if (bin->ref == ref)
	{
	  bin->count ++;
	  table->functions_modifying --;
	  return;
	}
      else
	{
	  bin->next = entry_create (ref);
	  table->entries ++;
	  table->functions_modifying --;
	  return;
	}
    }
}

static void
hash_table_del_ref (struct HashTable *table, const void *_ref)
{
  exit_if_not (table != NULL, "ReferenceTable.c:hash_table_del_ref needs a non null table.");
  table->functions_modifying ++;
  const _Object *ref = cast (_ref, Object);

  unsigned int index = hash_table_get_index (table, ref);
  struct Entry *bin = table->table[index];
  struct Entry *prev = NULL;

  while (bin)
    {
      if (bin->ref == ref)
	{
	  bin->count --;
	  if (bin->count == 0)
	    {
	      delete ((void *)bin->ref);
	      if (prev)
		{
		  prev->next = bin->next;
		}
	      else
		{
		  table->table[index] = bin->next;
		}

	      free (bin);
	      table->entries --;
	    }
	  table->functions_modifying --;
	  return;
	}

      prev = bin;
      bin = bin->next;
    }

  table->functions_modifying --;
}

static void
hash_table_copy (const struct HashTable *source, const struct HashTable *dest)
{
  for (int i = 0; i < source->size; i++)
    {
      struct Entry *bin = source->table[i];
      while (bin)
	{
	  hash_table_add_ref ((struct HashTable *)dest, bin->ref);
	  hash_table_set_count ((struct HashTable *)dest, bin->ref, bin->count);
	  bin = bin->next;
	}
    }
}

static void
close_reference_table ()
{
  if (!reference_table)
    return;

  for (int i = 0; i < reference_table->size; i++)
    {
      struct Entry *bin = reference_table->table[i];
      while (bin)
	{
	  struct Entry *next = bin->next;
	  delete ((void *)bin->ref);
	  free (bin);
	  bin = next;
	}
    }

  free (reference_table->table);
  free (reference_table);

  reference_table = NULL;
}

void *
ref (const _Object *ref)
{
  if (ref == NULL)
    return NULL;

  if (reference_table == NULL)
    {
      reference_table = hash_table_create (3);
      atexit (close_reference_table);
    }

  hash_table_add_ref (reference_table, ref);
  if (!reference_table->functions_modifying
      && reference_table->entries >= reference_table->size)
    {
      struct HashTable *new = hash_table_create (reference_table->size * 2);
      hash_table_copy (reference_table, new);
      hash_table_delete (reference_table);
      reference_table = new;
    }

  return (void *)ref;
}

void
unref (const _Object *ref)
{
  if (reference_table == NULL || ref == NULL)
    return;

  hash_table_del_ref (reference_table, ref);
  if (!reference_table->functions_modifying
      && reference_table->entries <= (reference_table->size / 4))
    {
      struct HashTable *new = hash_table_create (reference_table->size / 2);
      hash_table_copy (reference_table, new);
      hash_table_delete (reference_table);
      reference_table = new;
    }
}
