/********************************************************************
 *                                                                  *
 * THIS FILE IS PART OF THE OggVorbis 'TREMOR' CODEC SOURCE CODE.   *
 *                                                                  *
 * USE, DISTRIBUTION AND REPRODUCTION OF THIS LIBRARY SOURCE IS     *
 * GOVERNED BY A BSD-STYLE SOURCE LICENSE INCLUDED WITH THIS SOURCE *
 * IN 'COPYING'. PLEASE READ THESE TERMS BEFORE DISTRIBUTING.       *
 *                                                                  *
 * THE OggVorbis 'TREMOR' SOURCE CODE IS (C) COPYRIGHT 1994-2002    *
 * BY THE Xiph.Org FOUNDATION http://www.xiph.org/                  *
 *                                                                  *
 ********************************************************************

 function: sin,cos lookup tables

 ********************************************************************/
  
#include "os_types.h"

/* we keep the most used sin cosine table in fast IRAM;
   unfortunately, we don't have the space for both tables */

/* {sin(2*i*PI/4096), cos(2*i*PI/4096)}, with i = 0 to 512 */
static LOOKUP_T sincos_lookup0[1026] IDATA_ATTR = {
  X(0x00000000), X(0x7fffffff), X(0x003243f5), X(0x7ffff621),
  X(0x006487e3), X(0x7fffd886), X(0x0096cbc1), X(0x7fffa72c),
  X(0x00c90f88), X(0x7fff6216), X(0x00fb5330), X(0x7fff0943),
  X(0x012d96b1), X(0x7ffe9cb2), X(0x015fda03), X(0x7ffe1c65),
  X(0x01921d20), X(0x7ffd885a), X(0x01c45ffe), X(0x7ffce093),
  X(0x01f6a297), X(0x7ffc250f), X(0x0228e4e2), X(0x7ffb55ce),
  X(0x025b26d7), X(0x7ffa72d1), X(0x028d6870), X(0x7ff97c18),
  X(0x02bfa9a4), X(0x7ff871a2), X(0x02f1ea6c), X(0x7ff75370),
  X(0x03242abf), X(0x7ff62182), X(0x03566a96), X(0x7ff4dbd9),
  X(0x0388a9ea), X(0x7ff38274), X(0x03bae8b2), X(0x7ff21553),
  X(0x03ed26e6), X(0x7ff09478), X(0x041f6480), X(0x7feeffe1),
  X(0x0451a177), X(0x7fed5791), X(0x0483ddc3), X(0x7feb9b85),
  X(0x04b6195d), X(0x7fe9cbc0), X(0x04e8543e), X(0x7fe7e841),
  X(0x051a8e5c), X(0x7fe5f108), X(0x054cc7b1), X(0x7fe3e616),
  X(0x057f0035), X(0x7fe1c76b), X(0x05b137df), X(0x7fdf9508),
  X(0x05e36ea9), X(0x7fdd4eec), X(0x0615a48b), X(0x7fdaf519),
  X(0x0647d97c), X(0x7fd8878e), X(0x067a0d76), X(0x7fd6064c),
  X(0x06ac406f), X(0x7fd37153), X(0x06de7262), X(0x7fd0c8a3),
  X(0x0710a345), X(0x7fce0c3e), X(0x0742d311), X(0x7fcb3c23),
  X(0x077501be), X(0x7fc85854), X(0x07a72f45), X(0x7fc560cf),
  X(0x07d95b9e), X(0x7fc25596), X(0x080b86c2), X(0x7fbf36aa),
  X(0x083db0a7), X(0x7fbc040a), X(0x086fd947), X(0x7fb8bdb8),
  X(0x08a2009a), X(0x7fb563b3), X(0x08d42699), X(0x7fb1f5fc),
  X(0x09064b3a), X(0x7fae7495), X(0x09386e78), X(0x7faadf7c),
  X(0x096a9049), X(0x7fa736b4), X(0x099cb0a7), X(0x7fa37a3c),
  X(0x09cecf89), X(0x7f9faa15), X(0x0a00ece8), X(0x7f9bc640),
  X(0x0a3308bd), X(0x7f97cebd), X(0x0a6522fe), X(0x7f93c38c),
  X(0x0a973ba5), X(0x7f8fa4b0), X(0x0ac952aa), X(0x7f8b7227),
  X(0x0afb6805), X(0x7f872bf3), X(0x0b2d7baf), X(0x7f82d214),
  X(0x0b5f8d9f), X(0x7f7e648c), X(0x0b919dcf), X(0x7f79e35a),
  X(0x0bc3ac35), X(0x7f754e80), X(0x0bf5b8cb), X(0x7f70a5fe),
  X(0x0c27c389), X(0x7f6be9d4), X(0x0c59cc68), X(0x7f671a05),
  X(0x0c8bd35e), X(0x7f62368f), X(0x0cbdd865), X(0x7f5d3f75),
  X(0x0cefdb76), X(0x7f5834b7), X(0x0d21dc87), X(0x7f531655),
  X(0x0d53db92), X(0x7f4de451), X(0x0d85d88f), X(0x7f489eaa),
  X(0x0db7d376), X(0x7f434563), X(0x0de9cc40), X(0x7f3dd87c),
  X(0x0e1bc2e4), X(0x7f3857f6), X(0x0e4db75b), X(0x7f32c3d1),
  X(0x0e7fa99e), X(0x7f2d1c0e), X(0x0eb199a4), X(0x7f2760af),
  X(0x0ee38766), X(0x7f2191b4), X(0x0f1572dc), X(0x7f1baf1e),
  X(0x0f475bff), X(0x7f15b8ee), X(0x0f7942c7), X(0x7f0faf25),
  X(0x0fab272b), X(0x7f0991c4), X(0x0fdd0926), X(0x7f0360cb),
  X(0x100ee8ad), X(0x7efd1c3c), X(0x1040c5bb), X(0x7ef6c418),
  X(0x1072a048), X(0x7ef05860), X(0x10a4784b), X(0x7ee9d914),
  X(0x10d64dbd), X(0x7ee34636), X(0x11082096), X(0x7edc9fc6),
  X(0x1139f0cf), X(0x7ed5e5c6), X(0x116bbe60), X(0x7ecf1837),
  X(0x119d8941), X(0x7ec8371a), X(0x11cf516a), X(0x7ec14270),
  X(0x120116d5), X(0x7eba3a39), X(0x1232d979), X(0x7eb31e78),
  X(0x1264994e), X(0x7eabef2c), X(0x1296564d), X(0x7ea4ac58),
  X(0x12c8106f), X(0x7e9d55fc), X(0x12f9c7aa), X(0x7e95ec1a),
  X(0x132b7bf9), X(0x7e8e6eb2), X(0x135d2d53), X(0x7e86ddc6),
  X(0x138edbb1), X(0x7e7f3957), X(0x13c0870a), X(0x7e778166),
  X(0x13f22f58), X(0x7e6fb5f4), X(0x1423d492), X(0x7e67d703),
  X(0x145576b1), X(0x7e5fe493), X(0x148715ae), X(0x7e57dea7),
  X(0x14b8b17f), X(0x7e4fc53e), X(0x14ea4a1f), X(0x7e47985b),
  X(0x151bdf86), X(0x7e3f57ff), X(0x154d71aa), X(0x7e37042a),
  X(0x157f0086), X(0x7e2e9cdf), X(0x15b08c12), X(0x7e26221f),
  X(0x15e21445), X(0x7e1d93ea), X(0x16139918), X(0x7e14f242),
  X(0x16451a83), X(0x7e0c3d29), X(0x1676987f), X(0x7e0374a0),
  X(0x16a81305), X(0x7dfa98a8), X(0x16d98a0c), X(0x7df1a942),
  X(0x170afd8d), X(0x7de8a670), X(0x173c6d80), X(0x7ddf9034),
  X(0x176dd9de), X(0x7dd6668f), X(0x179f429f), X(0x7dcd2981),
  X(0x17d0a7bc), X(0x7dc3d90d), X(0x1802092c), X(0x7dba7534),
  X(0x183366e9), X(0x7db0fdf8), X(0x1864c0ea), X(0x7da77359),
  X(0x18961728), X(0x7d9dd55a), X(0x18c7699b), X(0x7d9423fc),
  X(0x18f8b83c), X(0x7d8a5f40), X(0x192a0304), X(0x7d808728),
  X(0x195b49ea), X(0x7d769bb5), X(0x198c8ce7), X(0x7d6c9ce9),
  X(0x19bdcbf3), X(0x7d628ac6), X(0x19ef0707), X(0x7d58654d),
  X(0x1a203e1b), X(0x7d4e2c7f), X(0x1a517128), X(0x7d43e05e),
  X(0x1a82a026), X(0x7d3980ec), X(0x1ab3cb0d), X(0x7d2f0e2b),
  X(0x1ae4f1d6), X(0x7d24881b), X(0x1b161479), X(0x7d19eebf),
  X(0x1b4732ef), X(0x7d0f4218), X(0x1b784d30), X(0x7d048228),
  X(0x1ba96335), X(0x7cf9aef0), X(0x1bda74f6), X(0x7ceec873),
  X(0x1c0b826a), X(0x7ce3ceb2), X(0x1c3c8b8c), X(0x7cd8c1ae),
  X(0x1c6d9053), X(0x7ccda169), X(0x1c9e90b8), X(0x7cc26de5),
  X(0x1ccf8cb3), X(0x7cb72724), X(0x1d00843d), X(0x7cabcd28),
  X(0x1d31774d), X(0x7ca05ff1), X(0x1d6265dd), X(0x7c94df83),
  X(0x1d934fe5), X(0x7c894bde), X(0x1dc4355e), X(0x7c7da505),
  X(0x1df5163f), X(0x7c71eaf9), X(0x1e25f282), X(0x7c661dbc),
  X(0x1e56ca1e), X(0x7c5a3d50), X(0x1e879d0d), X(0x7c4e49b7),
  X(0x1eb86b46), X(0x7c4242f2), X(0x1ee934c3), X(0x7c362904),
  X(0x1f19f97b), X(0x7c29fbee), X(0x1f4ab968), X(0x7c1dbbb3),
  X(0x1f7b7481), X(0x7c116853), X(0x1fac2abf), X(0x7c0501d2),
  X(0x1fdcdc1b), X(0x7bf88830), X(0x200d888d), X(0x7bebfb70),
  X(0x203e300d), X(0x7bdf5b94), X(0x206ed295), X(0x7bd2a89e),
  X(0x209f701c), X(0x7bc5e290), X(0x20d0089c), X(0x7bb9096b),
  X(0x21009c0c), X(0x7bac1d31), X(0x21312a65), X(0x7b9f1de6),
  X(0x2161b3a0), X(0x7b920b89), X(0x219237b5), X(0x7b84e61f),
  X(0x21c2b69c), X(0x7b77ada8), X(0x21f3304f), X(0x7b6a6227),
  X(0x2223a4c5), X(0x7b5d039e), X(0x225413f8), X(0x7b4f920e),
  X(0x22847de0), X(0x7b420d7a), X(0x22b4e274), X(0x7b3475e5),
  X(0x22e541af), X(0x7b26cb4f), X(0x23159b88), X(0x7b190dbc),
  X(0x2345eff8), X(0x7b0b3d2c), X(0x23763ef7), X(0x7afd59a4),
  X(0x23a6887f), X(0x7aef6323), X(0x23d6cc87), X(0x7ae159ae),
  X(0x24070b08), X(0x7ad33d45), X(0x243743fa), X(0x7ac50dec),
  X(0x24677758), X(0x7ab6cba4), X(0x2497a517), X(0x7aa8766f),
  X(0x24c7cd33), X(0x7a9a0e50), X(0x24f7efa2), X(0x7a8b9348),
  X(0x25280c5e), X(0x7a7d055b), X(0x2558235f), X(0x7a6e648a),
  X(0x2588349d), X(0x7a5fb0d8), X(0x25b84012), X(0x7a50ea47),
  X(0x25e845b6), X(0x7a4210d8), X(0x26184581), X(0x7a332490),
  X(0x26483f6c), X(0x7a24256f), X(0x26783370), X(0x7a151378),
  X(0x26a82186), X(0x7a05eead), X(0x26d809a5), X(0x79f6b711),
  X(0x2707ebc7), X(0x79e76ca7), X(0x2737c7e3), X(0x79d80f6f),
  X(0x27679df4), X(0x79c89f6e), X(0x27976df1), X(0x79b91ca4),
  X(0x27c737d3), X(0x79a98715), X(0x27f6fb92), X(0x7999dec4),
  X(0x2826b928), X(0x798a23b1), X(0x2856708d), X(0x797a55e0),
  X(0x288621b9), X(0x796a7554), X(0x28b5cca5), X(0x795a820e),
  X(0x28e5714b), X(0x794a7c12), X(0x29150fa1), X(0x793a6361),
  X(0x2944a7a2), X(0x792a37fe), X(0x29743946), X(0x7919f9ec),
  X(0x29a3c485), X(0x7909a92d), X(0x29d34958), X(0x78f945c3),
  X(0x2a02c7b8), X(0x78e8cfb2), X(0x2a323f9e), X(0x78d846fb),
  X(0x2a61b101), X(0x78c7aba2), X(0x2a911bdc), X(0x78b6fda8),
  X(0x2ac08026), X(0x78a63d11), X(0x2aefddd8), X(0x789569df),
  X(0x2b1f34eb), X(0x78848414), X(0x2b4e8558), X(0x78738bb3),
  X(0x2b7dcf17), X(0x786280bf), X(0x2bad1221), X(0x7851633b),
  X(0x2bdc4e6f), X(0x78403329), X(0x2c0b83fa), X(0x782ef08b),
  X(0x2c3ab2b9), X(0x781d9b65), X(0x2c69daa6), X(0x780c33b8),
  X(0x2c98fbba), X(0x77fab989), X(0x2cc815ee), X(0x77e92cd9),
  X(0x2cf72939), X(0x77d78daa), X(0x2d263596), X(0x77c5dc01),
  X(0x2d553afc), X(0x77b417df), X(0x2d843964), X(0x77a24148),
  X(0x2db330c7), X(0x7790583e), X(0x2de2211e), X(0x777e5cc3),
  X(0x2e110a62), X(0x776c4edb), X(0x2e3fec8b), X(0x775a2e89),
  X(0x2e6ec792), X(0x7747fbce), X(0x2e9d9b70), X(0x7735b6af),
  X(0x2ecc681e), X(0x77235f2d), X(0x2efb2d95), X(0x7710f54c),
  X(0x2f29ebcc), X(0x76fe790e), X(0x2f58a2be), X(0x76ebea77),
  X(0x2f875262), X(0x76d94989), X(0x2fb5fab2), X(0x76c69647),
  X(0x2fe49ba7), X(0x76b3d0b4), X(0x30133539), X(0x76a0f8d2),
  X(0x3041c761), X(0x768e0ea6), X(0x30705217), X(0x767b1231),
  X(0x309ed556), X(0x76680376), X(0x30cd5115), X(0x7654e279),
  X(0x30fbc54d), X(0x7641af3d), X(0x312a31f8), X(0x762e69c4),
  X(0x3158970e), X(0x761b1211), X(0x3186f487), X(0x7607a828),
  X(0x31b54a5e), X(0x75f42c0b), X(0x31e39889), X(0x75e09dbd),
  X(0x3211df04), X(0x75ccfd42), X(0x32401dc6), X(0x75b94a9c),
  X(0x326e54c7), X(0x75a585cf), X(0x329c8402), X(0x7591aedd),
  X(0x32caab6f), X(0x757dc5ca), X(0x32f8cb07), X(0x7569ca99),
  X(0x3326e2c3), X(0x7555bd4c), X(0x3354f29b), X(0x75419de7),
  X(0x3382fa88), X(0x752d6c6c), X(0x33b0fa84), X(0x751928e0),
  X(0x33def287), X(0x7504d345), X(0x340ce28b), X(0x74f06b9e),
  X(0x343aca87), X(0x74dbf1ef), X(0x3468aa76), X(0x74c7663a),
  X(0x34968250), X(0x74b2c884), X(0x34c4520d), X(0x749e18cd),
  X(0x34f219a8), X(0x7489571c), X(0x351fd918), X(0x74748371),
  X(0x354d9057), X(0x745f9dd1), X(0x357b3f5d), X(0x744aa63f),
  X(0x35a8e625), X(0x74359cbd), X(0x35d684a6), X(0x74208150),
  X(0x36041ad9), X(0x740b53fb), X(0x3631a8b8), X(0x73f614c0),
  X(0x365f2e3b), X(0x73e0c3a3), X(0x368cab5c), X(0x73cb60a8),
  X(0x36ba2014), X(0x73b5ebd1), X(0x36e78c5b), X(0x73a06522),
  X(0x3714f02a), X(0x738acc9e), X(0x37424b7b), X(0x73752249),
  X(0x376f9e46), X(0x735f6626), X(0x379ce885), X(0x73499838),
  X(0x37ca2a30), X(0x7333b883), X(0x37f76341), X(0x731dc70a),
  X(0x382493b0), X(0x7307c3d0), X(0x3851bb77), X(0x72f1aed9),
  X(0x387eda8e), X(0x72db8828), X(0x38abf0ef), X(0x72c54fc1),
  X(0x38d8fe93), X(0x72af05a7), X(0x39060373), X(0x7298a9dd),
  X(0x3932ff87), X(0x72823c67), X(0x395ff2c9), X(0x726bbd48),
  X(0x398cdd32), X(0x72552c85), X(0x39b9bebc), X(0x723e8a20),
  X(0x39e6975e), X(0x7227d61c), X(0x3a136712), X(0x7211107e),
  X(0x3a402dd2), X(0x71fa3949), X(0x3a6ceb96), X(0x71e35080),
  X(0x3a99a057), X(0x71cc5626), X(0x3ac64c0f), X(0x71b54a41),
  X(0x3af2eeb7), X(0x719e2cd2), X(0x3b1f8848), X(0x7186fdde),
  X(0x3b4c18ba), X(0x716fbd68), X(0x3b78a007), X(0x71586b74),
  X(0x3ba51e29), X(0x71410805), X(0x3bd19318), X(0x7129931f),
  X(0x3bfdfecd), X(0x71120cc5), X(0x3c2a6142), X(0x70fa74fc),
  X(0x3c56ba70), X(0x70e2cbc6), X(0x3c830a50), X(0x70cb1128),
  X(0x3caf50da), X(0x70b34525), X(0x3cdb8e09), X(0x709b67c0),
  X(0x3d07c1d6), X(0x708378ff), X(0x3d33ec39), X(0x706b78e3),
  X(0x3d600d2c), X(0x70536771), X(0x3d8c24a8), X(0x703b44ad),
  X(0x3db832a6), X(0x7023109a), X(0x3de4371f), X(0x700acb3c),
  X(0x3e10320d), X(0x6ff27497), X(0x3e3c2369), X(0x6fda0cae),
  X(0x3e680b2c), X(0x6fc19385), X(0x3e93e950), X(0x6fa90921),
  X(0x3ebfbdcd), X(0x6f906d84), X(0x3eeb889c), X(0x6f77c0b3),
  X(0x3f1749b8), X(0x6f5f02b2), X(0x3f430119), X(0x6f463383),
  X(0x3f6eaeb8), X(0x6f2d532c), X(0x3f9a5290), X(0x6f1461b0),
  X(0x3fc5ec98), X(0x6efb5f12), X(0x3ff17cca), X(0x6ee24b57),
  X(0x401d0321), X(0x6ec92683), X(0x40487f94), X(0x6eaff099),
  X(0x4073f21d), X(0x6e96a99d), X(0x409f5ab6), X(0x6e7d5193),
  X(0x40cab958), X(0x6e63e87f), X(0x40f60dfb), X(0x6e4a6e66),
  X(0x4121589b), X(0x6e30e34a), X(0x414c992f), X(0x6e174730),
  X(0x4177cfb1), X(0x6dfd9a1c), X(0x41a2fc1a), X(0x6de3dc11),
  X(0x41ce1e65), X(0x6dca0d14), X(0x41f93689), X(0x6db02d29),
  X(0x42244481), X(0x6d963c54), X(0x424f4845), X(0x6d7c3a98),
  X(0x427a41d0), X(0x6d6227fa), X(0x42a5311b), X(0x6d48047e),
  X(0x42d0161e), X(0x6d2dd027), X(0x42faf0d4), X(0x6d138afb),
  X(0x4325c135), X(0x6cf934fc), X(0x4350873c), X(0x6cdece2f),
  X(0x437b42e1), X(0x6cc45698), X(0x43a5f41e), X(0x6ca9ce3b),
  X(0x43d09aed), X(0x6c8f351c), X(0x43fb3746), X(0x6c748b3f),
  X(0x4425c923), X(0x6c59d0a9), X(0x4450507e), X(0x6c3f055d),
  X(0x447acd50), X(0x6c242960), X(0x44a53f93), X(0x6c093cb6),
  X(0x44cfa740), X(0x6bee3f62), X(0x44fa0450), X(0x6bd3316a),
  X(0x452456bd), X(0x6bb812d1), X(0x454e9e80), X(0x6b9ce39b),
  X(0x4578db93), X(0x6b81a3cd), X(0x45a30df0), X(0x6b66536b),
  X(0x45cd358f), X(0x6b4af279), X(0x45f7526b), X(0x6b2f80fb),
  X(0x4621647d), X(0x6b13fef5), X(0x464b6bbe), X(0x6af86c6c),
  X(0x46756828), X(0x6adcc964), X(0x469f59b4), X(0x6ac115e2),
  X(0x46c9405c), X(0x6aa551e9), X(0x46f31c1a), X(0x6a897d7d),
  X(0x471cece7), X(0x6a6d98a4), X(0x4746b2bc), X(0x6a51a361),
  X(0x47706d93), X(0x6a359db9), X(0x479a1d67), X(0x6a1987b0),
  X(0x47c3c22f), X(0x69fd614a), X(0x47ed5be6), X(0x69e12a8c),
  X(0x4816ea86), X(0x69c4e37a), X(0x48406e08), X(0x69a88c19),
  X(0x4869e665), X(0x698c246c), X(0x48935397), X(0x696fac78),
  X(0x48bcb599), X(0x69532442), X(0x48e60c62), X(0x69368bce),
  X(0x490f57ee), X(0x6919e320), X(0x49389836), X(0x68fd2a3d),
  X(0x4961cd33), X(0x68e06129), X(0x498af6df), X(0x68c387e9),
  X(0x49b41533), X(0x68a69e81), X(0x49dd282a), X(0x6889a4f6),
  X(0x4a062fbd), X(0x686c9b4b), X(0x4a2f2be6), X(0x684f8186),
  X(0x4a581c9e), X(0x683257ab), X(0x4a8101de), X(0x68151dbe),
  X(0x4aa9dba2), X(0x67f7d3c5), X(0x4ad2a9e2), X(0x67da79c3),
  X(0x4afb6c98), X(0x67bd0fbd), X(0x4b2423be), X(0x679f95b7),
  X(0x4b4ccf4d), X(0x67820bb7), X(0x4b756f40), X(0x676471c0),
  X(0x4b9e0390), X(0x6746c7d8), X(0x4bc68c36), X(0x67290e02),
  X(0x4bef092d), X(0x670b4444), X(0x4c177a6e), X(0x66ed6aa1),
  X(0x4c3fdff4), X(0x66cf8120), X(0x4c6839b7), X(0x66b187c3),
  X(0x4c9087b1), X(0x66937e91), X(0x4cb8c9dd), X(0x6675658c),
  X(0x4ce10034), X(0x66573cbb), X(0x4d092ab0), X(0x66390422),
  X(0x4d31494b), X(0x661abbc5), X(0x4d595bfe), X(0x65fc63a9),
  X(0x4d8162c4), X(0x65ddfbd3), X(0x4da95d96), X(0x65bf8447),
  X(0x4dd14c6e), X(0x65a0fd0b), X(0x4df92f46), X(0x65826622),
  X(0x4e210617), X(0x6563bf92), X(0x4e48d0dd), X(0x6545095f),
  X(0x4e708f8f), X(0x6526438f), X(0x4e984229), X(0x65076e25),
  X(0x4ebfe8a5), X(0x64e88926), X(0x4ee782fb), X(0x64c99498),
  X(0x4f0f1126), X(0x64aa907f), X(0x4f369320), X(0x648b7ce0),
  X(0x4f5e08e3), X(0x646c59bf), X(0x4f857269), X(0x644d2722),
  X(0x4faccfab), X(0x642de50d), X(0x4fd420a4), X(0x640e9386),
  X(0x4ffb654d), X(0x63ef3290), X(0x50229da1), X(0x63cfc231),
  X(0x5049c999), X(0x63b0426d), X(0x5070e92f), X(0x6390b34a),
  X(0x5097fc5e), X(0x637114cc), X(0x50bf031f), X(0x635166f9),
  X(0x50e5fd6d), X(0x6331a9d4), X(0x510ceb40), X(0x6311dd64),
  X(0x5133cc94), X(0x62f201ac), X(0x515aa162), X(0x62d216b3),
  X(0x518169a5), X(0x62b21c7b), X(0x51a82555), X(0x6292130c),
  X(0x51ced46e), X(0x6271fa69), X(0x51f576ea), X(0x6251d298),
  X(0x521c0cc2), X(0x62319b9d), X(0x524295f0), X(0x6211557e),
  X(0x5269126e), X(0x61f1003f), X(0x528f8238), X(0x61d09be5),
  X(0x52b5e546), X(0x61b02876), X(0x52dc3b92), X(0x618fa5f7),
  X(0x53028518), X(0x616f146c), X(0x5328c1d0), X(0x614e73da),
  X(0x534ef1b5), X(0x612dc447), X(0x537514c2), X(0x610d05b7),
  X(0x539b2af0), X(0x60ec3830), X(0x53c13439), X(0x60cb5bb7),
  X(0x53e73097), X(0x60aa7050), X(0x540d2005), X(0x60897601),
  X(0x5433027d), X(0x60686ccf), X(0x5458d7f9), X(0x604754bf),
  X(0x547ea073), X(0x60262dd6), X(0x54a45be6), X(0x6004f819),
  X(0x54ca0a4b), X(0x5fe3b38d), X(0x54efab9c), X(0x5fc26038),
  X(0x55153fd4), X(0x5fa0fe1f), X(0x553ac6ee), X(0x5f7f8d46),
  X(0x556040e2), X(0x5f5e0db3), X(0x5585adad), X(0x5f3c7f6b),
  X(0x55ab0d46), X(0x5f1ae274), X(0x55d05faa), X(0x5ef936d1),
  X(0x55f5a4d2), X(0x5ed77c8a), X(0x561adcb9), X(0x5eb5b3a2),
  X(0x56400758), X(0x5e93dc1f), X(0x566524aa), X(0x5e71f606),
  X(0x568a34a9), X(0x5e50015d), X(0x56af3750), X(0x5e2dfe29),
  X(0x56d42c99), X(0x5e0bec6e), X(0x56f9147e), X(0x5de9cc33),
  X(0x571deefa), X(0x5dc79d7c), X(0x5742bc06), X(0x5da5604f),
  X(0x57677b9d), X(0x5d8314b1), X(0x578c2dba), X(0x5d60baa7),
  X(0x57b0d256), X(0x5d3e5237), X(0x57d5696d), X(0x5d1bdb65),
  X(0x57f9f2f8), X(0x5cf95638), X(0x581e6ef1), X(0x5cd6c2b5),
  X(0x5842dd54), X(0x5cb420e0), X(0x58673e1b), X(0x5c9170bf),
  X(0x588b9140), X(0x5c6eb258), X(0x58afd6bd), X(0x5c4be5b0),
  X(0x58d40e8c), X(0x5c290acc), X(0x58f838a9), X(0x5c0621b2),
  X(0x591c550e), X(0x5be32a67), X(0x594063b5), X(0x5bc024f0),
  X(0x59646498), X(0x5b9d1154), X(0x598857b2), X(0x5b79ef96),
  X(0x59ac3cfd), X(0x5b56bfbd), X(0x59d01475), X(0x5b3381ce),
  X(0x59f3de12), X(0x5b1035cf), X(0x5a1799d1), X(0x5aecdbc5),
  X(0x5a3b47ab), X(0x5ac973b5), X(0x5a5ee79a), X(0x5aa5fda5),
  X(0x5a82799a), X(0x5a82799a)
  };
  
  /* {sin((2*i+1)*PI/4096), cos((2*i+1)*PI/4096)}, with i = 0 to 511 */
static LOOKUP_T sincos_lookup1[1024] = {
  X(0x001921fb), X(0x7ffffd88), X(0x004b65ee), X(0x7fffe9cb),
  X(0x007da9d4), X(0x7fffc251), X(0x00afeda8), X(0x7fff8719),
  X(0x00e23160), X(0x7fff3824), X(0x011474f6), X(0x7ffed572),
  X(0x0146b860), X(0x7ffe5f03), X(0x0178fb99), X(0x7ffdd4d7),
  X(0x01ab3e97), X(0x7ffd36ee), X(0x01dd8154), X(0x7ffc8549),
  X(0x020fc3c6), X(0x7ffbbfe6), X(0x024205e8), X(0x7ffae6c7),
  X(0x027447b0), X(0x7ff9f9ec), X(0x02a68917), X(0x7ff8f954),
  X(0x02d8ca16), X(0x7ff7e500), X(0x030b0aa4), X(0x7ff6bcf0),
  X(0x033d4abb), X(0x7ff58125), X(0x036f8a51), X(0x7ff4319d),
  X(0x03a1c960), X(0x7ff2ce5b), X(0x03d407df), X(0x7ff1575d),
  X(0x040645c7), X(0x7fefcca4), X(0x04388310), X(0x7fee2e30),
  X(0x046abfb3), X(0x7fec7c02), X(0x049cfba7), X(0x7feab61a),
  X(0x04cf36e5), X(0x7fe8dc78), X(0x05017165), X(0x7fe6ef1c),
  X(0x0533ab20), X(0x7fe4ee06), X(0x0565e40d), X(0x7fe2d938),
  X(0x05981c26), X(0x7fe0b0b1), X(0x05ca5361), X(0x7fde7471),
  X(0x05fc89b8), X(0x7fdc247a), X(0x062ebf22), X(0x7fd9c0ca),
  X(0x0660f398), X(0x7fd74964), X(0x06932713), X(0x7fd4be46),
  X(0x06c5598a), X(0x7fd21f72), X(0x06f78af6), X(0x7fcf6ce8),
  X(0x0729bb4e), X(0x7fcca6a7), X(0x075bea8c), X(0x7fc9ccb2),
  X(0x078e18a7), X(0x7fc6df08), X(0x07c04598), X(0x7fc3dda9),
  X(0x07f27157), X(0x7fc0c896), X(0x08249bdd), X(0x7fbd9fd0),
  X(0x0856c520), X(0x7fba6357), X(0x0888ed1b), X(0x7fb7132b),
  X(0x08bb13c5), X(0x7fb3af4e), X(0x08ed3916), X(0x7fb037bf),
  X(0x091f5d06), X(0x7facac7f), X(0x09517f8f), X(0x7fa90d8e),
  X(0x0983a0a7), X(0x7fa55aee), X(0x09b5c048), X(0x7fa1949e),
  X(0x09e7de6a), X(0x7f9dbaa0), X(0x0a19fb04), X(0x7f99ccf4),
  X(0x0a4c1610), X(0x7f95cb9a), X(0x0a7e2f85), X(0x7f91b694),
  X(0x0ab0475c), X(0x7f8d8de1), X(0x0ae25d8d), X(0x7f895182),
  X(0x0b147211), X(0x7f850179), X(0x0b4684df), X(0x7f809dc5),
  X(0x0b7895f0), X(0x7f7c2668), X(0x0baaa53b), X(0x7f779b62),
  X(0x0bdcb2bb), X(0x7f72fcb4), X(0x0c0ebe66), X(0x7f6e4a5e),
  X(0x0c40c835), X(0x7f698461), X(0x0c72d020), X(0x7f64aabf),
  X(0x0ca4d620), X(0x7f5fbd77), X(0x0cd6da2d), X(0x7f5abc8a),
  X(0x0d08dc3f), X(0x7f55a7fa), X(0x0d3adc4e), X(0x7f507fc7),
  X(0x0d6cda53), X(0x7f4b43f2), X(0x0d9ed646), X(0x7f45f47b),
  X(0x0dd0d01f), X(0x7f409164), X(0x0e02c7d7), X(0x7f3b1aad),
  X(0x0e34bd66), X(0x7f359057), X(0x0e66b0c3), X(0x7f2ff263),
  X(0x0e98a1e9), X(0x7f2a40d2), X(0x0eca90ce), X(0x7f247ba5),
  X(0x0efc7d6b), X(0x7f1ea2dc), X(0x0f2e67b8), X(0x7f18b679),
  X(0x0f604faf), X(0x7f12b67c), X(0x0f923546), X(0x7f0ca2e7),
  X(0x0fc41876), X(0x7f067bba), X(0x0ff5f938), X(0x7f0040f6),
  X(0x1027d784), X(0x7ef9f29d), X(0x1059b352), X(0x7ef390ae),
  X(0x108b8c9b), X(0x7eed1b2c), X(0x10bd6356), X(0x7ee69217),
  X(0x10ef377d), X(0x7edff570), X(0x11210907), X(0x7ed94538),
  X(0x1152d7ed), X(0x7ed28171), X(0x1184a427), X(0x7ecbaa1a),
  X(0x11b66dad), X(0x7ec4bf36), X(0x11e83478), X(0x7ebdc0c6),
  X(0x1219f880), X(0x7eb6aeca), X(0x124bb9be), X(0x7eaf8943),
  X(0x127d7829), X(0x7ea85033), X(0x12af33ba), X(0x7ea1039b),
  X(0x12e0ec6a), X(0x7e99a37c), X(0x1312a230), X(0x7e922fd6),
  X(0x13445505), X(0x7e8aa8ac), X(0x137604e2), X(0x7e830dff),
  X(0x13a7b1bf), X(0x7e7b5fce), X(0x13d95b93), X(0x7e739e1d),
  X(0x140b0258), X(0x7e6bc8eb), X(0x143ca605), X(0x7e63e03b),
  X(0x146e4694), X(0x7e5be40c), X(0x149fe3fc), X(0x7e53d462),
  X(0x14d17e36), X(0x7e4bb13c), X(0x1503153a), X(0x7e437a9c),
  X(0x1534a901), X(0x7e3b3083), X(0x15663982), X(0x7e32d2f4),
  X(0x1597c6b7), X(0x7e2a61ed), X(0x15c95097), X(0x7e21dd73),
  X(0x15fad71b), X(0x7e194584), X(0x162c5a3b), X(0x7e109a24),
  X(0x165dd9f0), X(0x7e07db52), X(0x168f5632), X(0x7dff0911),
  X(0x16c0cef9), X(0x7df62362), X(0x16f2443e), X(0x7ded2a47),
  X(0x1723b5f9), X(0x7de41dc0), X(0x17552422), X(0x7ddafdce),
  X(0x17868eb3), X(0x7dd1ca75), X(0x17b7f5a3), X(0x7dc883b4),
  X(0x17e958ea), X(0x7dbf298d), X(0x181ab881), X(0x7db5bc02),
  X(0x184c1461), X(0x7dac3b15), X(0x187d6c82), X(0x7da2a6c6),
  X(0x18aec0db), X(0x7d98ff17), X(0x18e01167), X(0x7d8f4409),
  X(0x19115e1c), X(0x7d85759f), X(0x1942a6f3), X(0x7d7b93da),
  X(0x1973ebe6), X(0x7d719eba), X(0x19a52ceb), X(0x7d679642),
  X(0x19d669fc), X(0x7d5d7a74), X(0x1a07a311), X(0x7d534b50),
  X(0x1a38d823), X(0x7d4908d9), X(0x1a6a0929), X(0x7d3eb30f),
  X(0x1a9b361d), X(0x7d3449f5), X(0x1acc5ef6), X(0x7d29cd8c),
  X(0x1afd83ad), X(0x7d1f3dd6), X(0x1b2ea43a), X(0x7d149ad5),
  X(0x1b5fc097), X(0x7d09e489), X(0x1b90d8bb), X(0x7cff1af5),
  X(0x1bc1ec9e), X(0x7cf43e1a), X(0x1bf2fc3a), X(0x7ce94dfb),
  X(0x1c240786), X(0x7cde4a98), X(0x1c550e7c), X(0x7cd333f3),
  X(0x1c861113), X(0x7cc80a0f), X(0x1cb70f43), X(0x7cbcccec),
  X(0x1ce80906), X(0x7cb17c8d), X(0x1d18fe54), X(0x7ca618f3),
  X(0x1d49ef26), X(0x7c9aa221), X(0x1d7adb73), X(0x7c8f1817),
  X(0x1dabc334), X(0x7c837ad8), X(0x1ddca662), X(0x7c77ca65),
  X(0x1e0d84f5), X(0x7c6c06c0), X(0x1e3e5ee5), X(0x7c602fec),
  X(0x1e6f342c), X(0x7c5445e9), X(0x1ea004c1), X(0x7c4848ba),
  X(0x1ed0d09d), X(0x7c3c3860), X(0x1f0197b8), X(0x7c3014de),
  X(0x1f325a0b), X(0x7c23de35), X(0x1f63178f), X(0x7c179467),
  X(0x1f93d03c), X(0x7c0b3777), X(0x1fc4840a), X(0x7bfec765),
  X(0x1ff532f2), X(0x7bf24434), X(0x2025dcec), X(0x7be5ade6),
  X(0x205681f1), X(0x7bd9047c), X(0x208721f9), X(0x7bcc47fa),
  X(0x20b7bcfe), X(0x7bbf7860), X(0x20e852f6), X(0x7bb295b0),
  X(0x2118e3dc), X(0x7ba59fee), X(0x21496fa7), X(0x7b989719),
  X(0x2179f64f), X(0x7b8b7b36), X(0x21aa77cf), X(0x7b7e4c45),
  X(0x21daf41d), X(0x7b710a49), X(0x220b6b32), X(0x7b63b543),
  X(0x223bdd08), X(0x7b564d36), X(0x226c4996), X(0x7b48d225),
  X(0x229cb0d5), X(0x7b3b4410), X(0x22cd12bd), X(0x7b2da2fa),
  X(0x22fd6f48), X(0x7b1feee5), X(0x232dc66d), X(0x7b1227d3),
  X(0x235e1826), X(0x7b044dc7), X(0x238e646a), X(0x7af660c2),
  X(0x23beab33), X(0x7ae860c7), X(0x23eeec78), X(0x7ada4dd8),
  X(0x241f2833), X(0x7acc27f7), X(0x244f5e5c), X(0x7abdef25),
  X(0x247f8eec), X(0x7aafa367), X(0x24afb9da), X(0x7aa144bc),
  X(0x24dfdf20), X(0x7a92d329), X(0x250ffeb7), X(0x7a844eae),
  X(0x25401896), X(0x7a75b74f), X(0x25702cb7), X(0x7a670d0d),
  X(0x25a03b11), X(0x7a584feb), X(0x25d0439f), X(0x7a497feb),
  X(0x26004657), X(0x7a3a9d0f), X(0x26304333), X(0x7a2ba75a),
  X(0x26603a2c), X(0x7a1c9ece), X(0x26902b39), X(0x7a0d836d),
  X(0x26c01655), X(0x79fe5539), X(0x26effb76), X(0x79ef1436),
  X(0x271fda96), X(0x79dfc064), X(0x274fb3ae), X(0x79d059c8),
  X(0x277f86b5), X(0x79c0e062), X(0x27af53a6), X(0x79b15435),
  X(0x27df1a77), X(0x79a1b545), X(0x280edb23), X(0x79920392),
  X(0x283e95a1), X(0x79823f20), X(0x286e49ea), X(0x797267f2),
  X(0x289df7f8), X(0x79627e08), X(0x28cd9fc1), X(0x79528167),
  X(0x28fd4140), X(0x79427210), X(0x292cdc6d), X(0x79325006),
  X(0x295c7140), X(0x79221b4b), X(0x298bffb2), X(0x7911d3e2),
  X(0x29bb87bc), X(0x790179cd), X(0x29eb0957), X(0x78f10d0f),
  X(0x2a1a847b), X(0x78e08dab), X(0x2a49f920), X(0x78cffba3),
  X(0x2a796740), X(0x78bf56f9), X(0x2aa8ced3), X(0x78ae9fb0),
  X(0x2ad82fd2), X(0x789dd5cb), X(0x2b078a36), X(0x788cf94c),
  X(0x2b36ddf7), X(0x787c0a36), X(0x2b662b0e), X(0x786b088c),
  X(0x2b957173), X(0x7859f44f), X(0x2bc4b120), X(0x7848cd83),
  X(0x2bf3ea0d), X(0x7837942b), X(0x2c231c33), X(0x78264849),
  X(0x2c52478a), X(0x7814e9df), X(0x2c816c0c), X(0x780378f1),
  X(0x2cb089b1), X(0x77f1f581), X(0x2cdfa071), X(0x77e05f91),
  X(0x2d0eb046), X(0x77ceb725), X(0x2d3db928), X(0x77bcfc3f),
  X(0x2d6cbb10), X(0x77ab2ee2), X(0x2d9bb5f6), X(0x77994f11),
  X(0x2dcaa9d5), X(0x77875cce), X(0x2df996a3), X(0x7775581d),
  X(0x2e287c5a), X(0x776340ff), X(0x2e575af3), X(0x77511778),
  X(0x2e863267), X(0x773edb8b), X(0x2eb502ae), X(0x772c8d3a),
  X(0x2ee3cbc1), X(0x771a2c88), X(0x2f128d99), X(0x7707b979),
  X(0x2f41482e), X(0x76f5340e), X(0x2f6ffb7a), X(0x76e29c4b),
  X(0x2f9ea775), X(0x76cff232), X(0x2fcd4c19), X(0x76bd35c7),
  X(0x2ffbe95d), X(0x76aa670d), X(0x302a7f3a), X(0x76978605),
  X(0x30590dab), X(0x768492b4), X(0x308794a6), X(0x76718d1c),
  X(0x30b61426), X(0x765e7540), X(0x30e48c22), X(0x764b4b23),
  X(0x3112fc95), X(0x76380ec8), X(0x31416576), X(0x7624c031),
  X(0x316fc6be), X(0x76115f63), X(0x319e2067), X(0x75fdec60),
  X(0x31cc7269), X(0x75ea672a), X(0x31fabcbd), X(0x75d6cfc5),
  X(0x3228ff5c), X(0x75c32634), X(0x32573a3f), X(0x75af6a7b),
  X(0x32856d5e), X(0x759b9c9b), X(0x32b398b3), X(0x7587bc98),
  X(0x32e1bc36), X(0x7573ca75), X(0x330fd7e1), X(0x755fc635),
  X(0x333debab), X(0x754bafdc), X(0x336bf78f), X(0x7537876c),
  X(0x3399fb85), X(0x75234ce8), X(0x33c7f785), X(0x750f0054),
  X(0x33f5eb89), X(0x74faa1b3), X(0x3423d78a), X(0x74e63108),
  X(0x3451bb81), X(0x74d1ae55), X(0x347f9766), X(0x74bd199f),
  X(0x34ad6b32), X(0x74a872e8), X(0x34db36df), X(0x7493ba34),
  X(0x3508fa66), X(0x747eef85), X(0x3536b5be), X(0x746a12df),
  X(0x356468e2), X(0x74552446), X(0x359213c9), X(0x744023bc),
  X(0x35bfb66e), X(0x742b1144), X(0x35ed50c9), X(0x7415ece2),
  X(0x361ae2d3), X(0x7400b69a), X(0x36486c86), X(0x73eb6e6e),
  X(0x3675edd9), X(0x73d61461), X(0x36a366c6), X(0x73c0a878),
  X(0x36d0d746), X(0x73ab2ab4), X(0x36fe3f52), X(0x73959b1b),
  X(0x372b9ee3), X(0x737ff9ae), X(0x3758f5f2), X(0x736a4671),
  X(0x37864477), X(0x73548168), X(0x37b38a6d), X(0x733eaa96),
  X(0x37e0c7cc), X(0x7328c1ff), X(0x380dfc8d), X(0x7312c7a5),
  X(0x383b28a9), X(0x72fcbb8c), X(0x38684c19), X(0x72e69db7),
  X(0x389566d6), X(0x72d06e2b), X(0x38c278d9), X(0x72ba2cea),
  X(0x38ef821c), X(0x72a3d9f7), X(0x391c8297), X(0x728d7557),
  X(0x39497a43), X(0x7276ff0d), X(0x39766919), X(0x7260771b),
  X(0x39a34f13), X(0x7249dd86), X(0x39d02c2a), X(0x72333251),
  X(0x39fd0056), X(0x721c7580), X(0x3a29cb91), X(0x7205a716),
  X(0x3a568dd4), X(0x71eec716), X(0x3a834717), X(0x71d7d585),
  X(0x3aaff755), X(0x71c0d265), X(0x3adc9e86), X(0x71a9bdba),
  X(0x3b093ca3), X(0x71929789), X(0x3b35d1a5), X(0x717b5fd3),
  X(0x3b625d86), X(0x7164169d), X(0x3b8ee03e), X(0x714cbbeb),
  X(0x3bbb59c7), X(0x71354fc0), X(0x3be7ca1a), X(0x711dd220),
  X(0x3c143130), X(0x7106430e), X(0x3c408f03), X(0x70eea28e),
  X(0x3c6ce38a), X(0x70d6f0a4), X(0x3c992ec0), X(0x70bf2d53),
  X(0x3cc5709e), X(0x70a7589f), X(0x3cf1a91c), X(0x708f728b),
  X(0x3d1dd835), X(0x70777b1c), X(0x3d49fde1), X(0x705f7255),
  X(0x3d761a19), X(0x70475839), X(0x3da22cd7), X(0x702f2ccd),
  X(0x3dce3614), X(0x7016f014), X(0x3dfa35c8), X(0x6ffea212),
  X(0x3e262bee), X(0x6fe642ca), X(0x3e52187f), X(0x6fcdd241),
  X(0x3e7dfb73), X(0x6fb5507a), X(0x3ea9d4c3), X(0x6f9cbd79),
  X(0x3ed5a46b), X(0x6f841942), X(0x3f016a61), X(0x6f6b63d8),
  X(0x3f2d26a0), X(0x6f529d40), X(0x3f58d921), X(0x6f39c57d),
  X(0x3f8481dd), X(0x6f20dc92), X(0x3fb020ce), X(0x6f07e285),
  X(0x3fdbb5ec), X(0x6eeed758), X(0x40074132), X(0x6ed5bb10),
  X(0x4032c297), X(0x6ebc8db0), X(0x405e3a16), X(0x6ea34f3d),
  X(0x4089a7a8), X(0x6e89ffb9), X(0x40b50b46), X(0x6e709f2a),
  X(0x40e064ea), X(0x6e572d93), X(0x410bb48c), X(0x6e3daaf8),
  X(0x4136fa27), X(0x6e24175c), X(0x416235b2), X(0x6e0a72c5),
  X(0x418d6729), X(0x6df0bd35), X(0x41b88e84), X(0x6dd6f6b1),
  X(0x41e3abbc), X(0x6dbd1f3c), X(0x420ebecb), X(0x6da336dc),
  X(0x4239c7aa), X(0x6d893d93), X(0x4264c653), X(0x6d6f3365),
  X(0x428fbabe), X(0x6d551858), X(0x42baa4e6), X(0x6d3aec6e),
  X(0x42e584c3), X(0x6d20afac), X(0x43105a50), X(0x6d066215),
  X(0x433b2585), X(0x6cec03af), X(0x4365e65b), X(0x6cd1947c),
  X(0x43909ccd), X(0x6cb71482), X(0x43bb48d4), X(0x6c9c83c3),
  X(0x43e5ea68), X(0x6c81e245), X(0x44108184), X(0x6c67300b),
  X(0x443b0e21), X(0x6c4c6d1a), X(0x44659039), X(0x6c319975),
  X(0x449007c4), X(0x6c16b521), X(0x44ba74bd), X(0x6bfbc021),
  X(0x44e4d71c), X(0x6be0ba7b), X(0x450f2edb), X(0x6bc5a431),
  X(0x45397bf4), X(0x6baa7d49), X(0x4563be60), X(0x6b8f45c7),
  X(0x458df619), X(0x6b73fdae), X(0x45b82318), X(0x6b58a503),
  X(0x45e24556), X(0x6b3d3bcb), X(0x460c5cce), X(0x6b21c208),
  X(0x46366978), X(0x6b0637c1), X(0x46606b4e), X(0x6aea9cf8),
  X(0x468a624a), X(0x6acef1b2), X(0x46b44e65), X(0x6ab335f4),
  X(0x46de2f99), X(0x6a9769c1), X(0x470805df), X(0x6a7b8d1e),
  X(0x4731d131), X(0x6a5fa010), X(0x475b9188), X(0x6a43a29a),
  X(0x478546de), X(0x6a2794c1), X(0x47aef12c), X(0x6a0b7689),
  X(0x47d8906d), X(0x69ef47f6), X(0x48022499), X(0x69d3090e),
  X(0x482badab), X(0x69b6b9d3), X(0x48552b9b), X(0x699a5a4c),
  X(0x487e9e64), X(0x697dea7b), X(0x48a805ff), X(0x69616a65),
  X(0x48d16265), X(0x6944da10), X(0x48fab391), X(0x6928397e),
  X(0x4923f97b), X(0x690b88b5), X(0x494d341e), X(0x68eec7b9),
  X(0x49766373), X(0x68d1f68f), X(0x499f8774), X(0x68b5153a),
  X(0x49c8a01b), X(0x689823bf), X(0x49f1ad61), X(0x687b2224),
  X(0x4a1aaf3f), X(0x685e106c), X(0x4a43a5b0), X(0x6840ee9b),
  X(0x4a6c90ad), X(0x6823bcb7), X(0x4a957030), X(0x68067ac3),
  X(0x4abe4433), X(0x67e928c5), X(0x4ae70caf), X(0x67cbc6c0),
  X(0x4b0fc99d), X(0x67ae54ba), X(0x4b387af9), X(0x6790d2b6),
  X(0x4b6120bb), X(0x677340ba), X(0x4b89badd), X(0x67559eca),
  X(0x4bb24958), X(0x6737ecea), X(0x4bdacc28), X(0x671a2b20),
  X(0x4c034345), X(0x66fc596f), X(0x4c2baea9), X(0x66de77dc),
  X(0x4c540e4e), X(0x66c0866d), X(0x4c7c622d), X(0x66a28524),
  X(0x4ca4aa41), X(0x66847408), X(0x4ccce684), X(0x6666531d),
  X(0x4cf516ee), X(0x66482267), X(0x4d1d3b7a), X(0x6629e1ec),
  X(0x4d455422), X(0x660b91af), X(0x4d6d60df), X(0x65ed31b5),
  X(0x4d9561ac), X(0x65cec204), X(0x4dbd5682), X(0x65b0429f),
  X(0x4de53f5a), X(0x6591b38c), X(0x4e0d1c30), X(0x657314cf),
  X(0x4e34ecfc), X(0x6554666d), X(0x4e5cb1b9), X(0x6535a86b),
  X(0x4e846a60), X(0x6516dacd), X(0x4eac16eb), X(0x64f7fd98),
  X(0x4ed3b755), X(0x64d910d1), X(0x4efb4b96), X(0x64ba147d),
  X(0x4f22d3aa), X(0x649b08a0), X(0x4f4a4f89), X(0x647bed3f),
  X(0x4f71bf2e), X(0x645cc260), X(0x4f992293), X(0x643d8806),
  X(0x4fc079b1), X(0x641e3e38), X(0x4fe7c483), X(0x63fee4f8),
  X(0x500f0302), X(0x63df7c4d), X(0x50363529), X(0x63c0043b),
  X(0x505d5af1), X(0x63a07cc7), X(0x50847454), X(0x6380e5f6),
  X(0x50ab814d), X(0x63613fcd), X(0x50d281d5), X(0x63418a50),
  X(0x50f975e6), X(0x6321c585), X(0x51205d7b), X(0x6301f171),
  X(0x5147388c), X(0x62e20e17), X(0x516e0715), X(0x62c21b7e),
  X(0x5194c910), X(0x62a219aa), X(0x51bb7e75), X(0x628208a1),
  X(0x51e22740), X(0x6261e866), X(0x5208c36a), X(0x6241b8ff),
  X(0x522f52ee), X(0x62217a72), X(0x5255d5c5), X(0x62012cc2),
  X(0x527c4bea), X(0x61e0cff5), X(0x52a2b556), X(0x61c06410),
  X(0x52c91204), X(0x619fe918), X(0x52ef61ee), X(0x617f5f12),
  X(0x5315a50e), X(0x615ec603), X(0x533bdb5d), X(0x613e1df0),
  X(0x536204d7), X(0x611d66de), X(0x53882175), X(0x60fca0d2),
  X(0x53ae3131), X(0x60dbcbd1), X(0x53d43406), X(0x60bae7e1),
  X(0x53fa29ed), X(0x6099f505), X(0x542012e1), X(0x6078f344),
  X(0x5445eedb), X(0x6057e2a2), X(0x546bbdd7), X(0x6036c325),
  X(0x54917fce), X(0x601594d1), X(0x54b734ba), X(0x5ff457ad),
  X(0x54dcdc96), X(0x5fd30bbc), X(0x5502775c), X(0x5fb1b104),
  X(0x55280505), X(0x5f90478a), X(0x554d858d), X(0x5f6ecf53),
  X(0x5572f8ed), X(0x5f4d4865), X(0x55985f20), X(0x5f2bb2c5),
  X(0x55bdb81f), X(0x5f0a0e77), X(0x55e303e6), X(0x5ee85b82),
  X(0x5608426e), X(0x5ec699e9), X(0x562d73b2), X(0x5ea4c9b3),
  X(0x565297ab), X(0x5e82eae5), X(0x5677ae54), X(0x5e60fd84),
  X(0x569cb7a8), X(0x5e3f0194), X(0x56c1b3a1), X(0x5e1cf71c),
  X(0x56e6a239), X(0x5dfade20), X(0x570b8369), X(0x5dd8b6a7),
  X(0x5730572e), X(0x5db680b4), X(0x57551d80), X(0x5d943c4e),
  X(0x5779d65b), X(0x5d71e979), X(0x579e81b8), X(0x5d4f883b),
  X(0x57c31f92), X(0x5d2d189a), X(0x57e7afe4), X(0x5d0a9a9a),
  X(0x580c32a7), X(0x5ce80e41), X(0x5830a7d6), X(0x5cc57394),
  X(0x58550f6c), X(0x5ca2ca99), X(0x58796962), X(0x5c801354),
  X(0x589db5b3), X(0x5c5d4dcc), X(0x58c1f45b), X(0x5c3a7a05),
  X(0x58e62552), X(0x5c179806), X(0x590a4893), X(0x5bf4a7d2),
  X(0x592e5e19), X(0x5bd1a971), X(0x595265df), X(0x5bae9ce7),
  X(0x59765fde), X(0x5b8b8239), X(0x599a4c12), X(0x5b68596d),
  X(0x59be2a74), X(0x5b452288), X(0x59e1faff), X(0x5b21dd90),
  X(0x5a05bdae), X(0x5afe8a8b), X(0x5a29727b), X(0x5adb297d),
  X(0x5a4d1960), X(0x5ab7ba6c), X(0x5a70b258), X(0x5a943d5e),
};
  
