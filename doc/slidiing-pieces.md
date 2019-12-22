# Sliding pieces move generation

Sliding pieces (rook, quuen) move generation usually requires the use of [magic bitboards](...), in the form of:

    TABLE[(BB * MASK) >> R_SHIFT]

Magic bitboards have great performance, but also tend to consume more memory then necessary. This is because, even though they employ perfect hashing, they rarely (if ever) are minimal.

What if we coud have a hash function that is both minimal and perfect? Ex., for A1 we would only have 49 occupancy configurations, compared to unoptimized 4096.

Possible ideas:

* Simply search for better magics. Hard to do and time-consuming. However, can be done in steps.
* Use best magics found so far by consulting the Wiki.
* Devise a different hash function.
* For now, use a simple hash table (with collisions, not minimal).
