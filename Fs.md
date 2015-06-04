# 1.Introduction #

**NOTICE: THE INFORMATION PROVIDED HERE IS NOT COMPLETE, AND MAY CHANGE AT ANY TIME.**

This documentation describes the file system designed and used in Micron System, and gives introduces how to use the facilitied to implement a new file system support.

# 2.Architecture #

The Micron System semi-realtime system implements UNIX-compliant file system and provides optimizations to solve some realtime issues, the structure of the design is as follows:

```
# File System Internal Structure
___________________________________________(File System)||(Device Module)__
[VFS Layer] >>>>>>>>>>>>>>>>>>>>>>>>>> |                ||
            > [Mount Point]            |                ||
            > [File System] > [MFS]    |                ||
                            > [E2FS]   |                ||
                            > [JFFS]   |                ||
                            > [FAT]    | [Sector Cache] || [Disk Driver]
            > [vnode Cache]            |                ||
```

The file system design used in Micron System implements 2 layers of caching, which provides senior effectiency in both inode management and disk read/write. The system is implemented in 3 layers and 4 parts:
  1. VFS Layer
  1. Mount Point
  1. Inode Cache
  1. Sector Cache

**Design Requirements:**
  1. As the file system may be need in device driver initialization time, to provide the same model of charateristic in both dynamically loaded and statically linked modules, any interface function or mechanisam implemented in File System should be initialization-less.
  1. File system implementation layer should also can be implented as modules, which are both statically and dynamically loadable during anytime of execution.
  1. As the need of meeting the general situations of controlling both caches, the file system implentations should all be non-specific both to mount points and devices underlying it.

# 3.File System Support Module Interface #

For each new file system's support in Micron System, the developer should write a kernel module interfacing the following structure, just like the other types of kernel modules and setup these entries in

```
struct fs fs[NFSENTYS];
```

These structures are defined in header file:

```
kernel\include\fs.h
```

```
struct fs
{
        /* file system probe */
        int (*probe)(dev_t dev);

        /* get file system block size */
        blkcnt_t (*blksz)(dev_t dev);

        /* load super block to buffer */
        int (*lsblk)(dev_t dev, char *buf);

        /* loading and saving nodes */
        int (*lnode)(void *sblk, struct vnode *node);
        int (*snode)(void *sblk, struct vnode *node);

        /* node allocation and releasing */
        struct vnode *(*ialloc)(void *sblk, dev_t dev);
        int (*ifree)(void *sblk, struct vnode *node);

        /* file data block allocation and releasing */
        blkcnt_t (*daddr)(void *sblk);
        blkcnt_t (*dalloc)(void *sblk, dev_t dev);
        int (*dfree)(void *sblk, dev_t dev, blkcnt_t blk);
};
```

For examples, you can refer to the file:
```
kernel\filesys\fs\mfs.c
kernel\filesys\fs\mfs.h
```
which is the native implementation of Micron File System and 100% match with the Micron System's VFS interface.

The key philosophy in adding a support for a new file system is **NODE DATA TRANSLATION**.

# 4.Sector Cache Interfaces #

_Sector cache here should actually be called block cache_

Sector caches can provide a middle layer in block device operations, which can moderately reduce the need to read data when it has been already read and unchanged, which can improve system performance by reducing I/O delays.

The reallocation of sector cache is based on Least Used Reallocation(LUR) policy.

The sector cache implements the following interfaces in Micron System:
```
/* These contents are defined in <scache.h> */
#define S_CHANGED       0x01    /* content has been changed, request commit */

/* core structure of sector cache */
struct scache
{
        blkcnt_t blk;           /* logical block number */
        unsigned sem;           /* in use count semaphore */
        char   buf[NMXBLKSZ];   /* the size is 1 block for all file systems */
        unsigned char mode;     /* mode bits of sector cache */
        struct scache *cnext;   /* cache list's next entry */
        struct scache *clast;   /* cache list's last entry */
        struct scache *anext;   /* allocation list's next entry */
        struct scache *alast;   /* allocation list's last entry */
};

/* sector buffer interface (based on blkdev) */
/* Get a block from dev, if not in cache, read it */
extern struct scache *scache_get(dev_t dev, blkcnt_t blk, blksize_t blksize);
/* sync a single specified block */
extern int scache_sync(dev_t dev, blkcnt_t blk, blksize_t blksize);
/* commit all changed blocks, which is marked with S_CHANGED macro */
extern int scache_commit(dev_t dev, blksize_t blksize);
```

# 5.Mount Point Table #

Mount point table is the mechanism which Micron used to register and mount a new block device, notice each device can be mounted ONLY ONCE.

```
/* These contents are defined in <mount.h> */
struct mountab
{
        char stat;                      /* mount point status */
        char sblk[NMXBLKSZ];            /* super block of mounted fs */
        blkcnt_t blksize;               /* device block size */
        enum fstype fs;                 /* file system type */
        dev_t hdev;                     /* device ID of host device */
        ino_t hino;                     /* inode ID of host device */
        struct vnode root;              /* root node of fs */
};

/* data */
extern struct mountab mountab[NVFSMNTS];

/* operations */
/* get system root (device) mount point */
extern struct mountab *mountab_getroot();       /* mountab[0] is root */
/* get dev's mount point structure */
extern struct mountab *mountab_get(dev_t dev);
/* seek for a mount point with host dev = hdev, host inode = hino */
extern struct mountab *mountab_seek(dev_t hdev, ino_t hino);
/* mount a device dev on host device hdev and host inode hino */
extern int mountab_mount(dev_t hdev, ino_t hino, dev_t dev);
/* dismount a device */
extern int mountab_umount(dev_t dev);
```

# 6.Vnode Cache #

Vnode cache provides cache mechanism on already read inodes, the allocation is based on Least Used Reallocation(LUR) policy.

```
/* These contents are defined in <vcache.h> */
/* reallocate a new vnode cache */
extern struct vnode *vcache_alloc();
/* promote a vnode if it has been used again and is not least used */
extern void vcache_promo(struct vnode *vp);
```