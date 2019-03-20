#ifndef _PID_H
#define _PID_H

#include "stm32f4xx_HAL.h"

#define SGN(x) ((x >= 0) ? 1 : -1)
#define ABS(x) (((x) >= 0)? (x) : -(x))
#define sin1000Hz {0.0, 0.006283143965558951, 0.012566039883352607, 0.018848439715408175, 0.02513009544333748, 0.03141075907812829, 0.03769018266993454, 0.0439681183178649, 0.050244318179769556, 0.05651853448202452, 0.06279051952931337, 0.06906002571440578, 0.07532680552793272, 0.08159061156815754, 0.08785119655074317, 0.09410831331851431, 0.1003617148512149, 0.10661115427525991, 0.11285638487348167, 0.11909716009486973, 0.12533323356430426, 0.1315643590922825, 0.13779029068463805, 0.14401078255225216, 0.15022558912075706, 0.15643446504023087, 0.16263716519488358, 0.16883344471273387, 0.17502305897527604, 0.18120576362713736, 0.1873813145857246, 0.19354946805086023, 0.19970998051440703, 0.20586260876988133, 0.21200710992205463, 0.21814324139654256, 0.22427076094938114, 0.23038942667659057, 0.23649899702372468, 0.2425992307954074, 0.2486898871648548, 0.25477072568338216, 0.26084150628989694, 0.2669019893203755, 0.27295193551732516, 0.2789911060392293, 0.2850192624699761, 0.2910361668282718, 0.2970415815770349, 0.30303526963277394, 0.3090169943749474, 0.3149865196553048, 0.3209436098072095, 0.32688802965494246, 0.3328195445229866, 0.33873792024529137, 0.34464292317451706, 0.350534320191259, 0.35641187871325075, 0.36227536670454563, 0.3681245526846779, 0.3739592057378004, 0.37977909552180106, 0.38558399227739654, 0.3913736668372024, 0.3971478906347806, 0.40290643571366264, 0.40864907473634904, 0.41437558099328414, 0.42008572841180625, 0.4257792915650727, 0.4314560456809589, 0.4371157666509328, 0.44275823103890144, 0.44838321609003223, 0.45399049973954675, 0.4595798606214878, 0.4651510780774583, 0.4707039321653325, 0.47623820366793906, 0.4817536741017153, 0.4872501257253323, 0.49272734154829156, 0.4981851053394908, 0.5036232016357608, 0.5090414157503713, 0.5144395337815063, 0.5198173426207094, 0.5251746299612956, 0.530511184306734, 0.5358267949789967, 0.5411212521268758, 0.5463943467342691, 0.5516458706284302, 0.556875616488188, 0.5620833778521306, 0.5672689491267565, 0.5724321255945909, 0.5775727034222675, 0.5826904796685761, 0.5877852522924731, 0.5928568201610592, 0.5979049830575188, 0.6029295416890247, 0.6079302976946054, 0.6129070536529764, 0.6178596130903343, 0.6227877804881126, 0.6276913612907005, 0.6325701619131244, 0.6374239897486896, 0.6422526531765844, 0.6470559615694442, 0.6518337253008788, 0.6565857557529565, 0.6613118653236518, 0.6660118674342517, 0.67068557653672, 0.6753328081210244, 0.6799533787224191, 0.6845471059286886, 0.6891138083873485, 0.6936533058128049, 0.6981654189934726, 0.7026499697988492, 0.7071067811865476, 0.7115356772092853, 0.7159364830218311, 0.7203090248879069, 0.7246531301870466, 0.7289686274214116, 0.7332553462225601, 0.7375131173581739, 0.7417417727387392, 0.7459411454241821, 0.7501110696304596, 0.7542513807361038, 0.7583619152887219, 0.7624425110114479, 0.7664930068093498, 0.7705132427757893, 0.7745030601987338, 0.7784623015670233, 0.7823908105765881, 0.7862884321366188, 0.7901550123756903, 0.7939903986478353, 0.797794439538571, 0.8015669848708765, 0.8053078857111219, 0.8090169943749475, 0.812694164433094, 0.8163392507171839, 0.8199521093254523, 0.8235325976284275, 0.8270805742745618, 0.8305958991958126, 0.8340784336131711, 0.8375280400421417, 0.840944582298169, 0.8443279255020151, 0.8476779360850831, 0.8509944817946918, 0.8542774316992952, 0.8575266561936523, 0.8607420270039436, 0.8639234171928353, 0.86707070116449, 0.8701837546695257, 0.8732624548099202, 0.8763066800438637, 0.8793163101905562, 0.8822912264349532, 0.8852313113324551, 0.8881364488135445, 0.8910065241883678, 0.8938414241512637, 0.8966410367852358, 0.899405251566371, 0.9021339593682028, 0.9048270524660196, 0.9074844245411169, 0.9101059706849957, 0.9126915874035028, 0.9152411726209175, 0.9177546256839811, 0.9202318473658703, 0.9226727398701148, 0.925077206834458, 0.9274451533346614, 0.9297764858882513, 0.932071112458211, 0.934328942456612, 0.9365498867481924, 0.9387338576538741, 0.9408807689542255, 0.9429905358928644, 0.9450630751798048, 0.9470983049947443, 0.9490961449902946, 0.9510565162951535, 0.9529793415172189, 0.954864544746643, 0.9567120515588305, 0.9585217890173758, 0.960293685676943, 0.9620276715860858, 0.9637236782900097, 0.9653816388332739, 0.967001487762435, 0.9685831611286311, 0.9701265964901058, 0.9716317329146739, 0.9730985109821265, 0.9745268727865771, 0.9759167619387473, 0.9772681235681935, 0.9785809043254721, 0.9798550523842469, 0.9810905174433341, 0.9822872507286886, 0.9834452049953297, 0.9845643345292053, 0.985644595148998, 0.986685944207868, 0.9876883405951378, 0.9886517447379141, 0.9895761186026509, 0.9904614256966512, 0.9913076310695066, 0.9921147013144779, 0.9928826045698137, 0.9936113105200084, 0.9943007903969989, 0.9949510169813002, 0.99556196460308, 0.9961336091431725, 0.9966659280340299, 0.9971589002606139, 0.9976125063612252, 0.9980267284282716, 0.998401550108975, 0.9987369566060175, 0.9990329346781247, 0.9992894726405892, 0.9995065603657316, 0.9996841892832999, 0.999822352380809, 0.9999210442038161, 0.9999802608561371, 1.0, 0.9999802608561371, 0.9999210442038161, 0.999822352380809, 0.9996841892832999, 0.9995065603657316, 0.9992894726405892, 0.9990329346781247, 0.9987369566060175, 0.998401550108975, 0.9980267284282716, 0.9976125063612252, 0.9971589002606139, 0.9966659280340299, 0.9961336091431725, 0.99556196460308, 0.9949510169813002, 0.9943007903969989, 0.9936113105200084, 0.9928826045698137, 0.9921147013144778, 0.9913076310695066, 0.9904614256966512, 0.9895761186026509, 0.988651744737914, 0.9876883405951377, 0.986685944207868, 0.985644595148998, 0.9845643345292053, 0.9834452049953296, 0.9822872507286886, 0.9810905174433341, 0.9798550523842469, 0.9785809043254722, 0.9772681235681935, 0.9759167619387474, 0.9745268727865772, 0.9730985109821266, 0.971631732914674, 0.9701265964901059, 0.9685831611286312, 0.9670014877624351, 0.965381638833274, 0.9637236782900097, 0.9620276715860859, 0.9602936856769431, 0.958521789017376, 0.9567120515588305, 0.9548645447466431, 0.9529793415172189, 0.9510565162951536, 0.9490961449902947, 0.9470983049947443, 0.9450630751798049, 0.9429905358928645, 0.9408807689542255, 0.9387338576538742, 0.9365498867481924, 0.9343289424566121, 0.932071112458211, 0.9297764858882515, 0.9274451533346614, 0.925077206834458, 0.9226727398701149, 0.9202318473658704, 0.9177546256839813, 0.9152411726209176, 0.9126915874035029, 0.9101059706849957, 0.9074844245411169, 0.9048270524660195, 0.9021339593682027, 0.8994052515663712, 0.896641036785236, 0.8938414241512639, 0.8910065241883679, 0.8881364488135446, 0.8852313113324553, 0.8822912264349533, 0.8793163101905562, 0.8763066800438635, 0.87326245480992, 0.8701837546695257, 0.8670707011644901, 0.8639234171928354, 0.8607420270039436, 0.8575266561936522, 0.8542774316992952, 0.8509944817946917, 0.8476779360850831, 0.844327925502015, 0.8409445822981692, 0.8375280400421418, 0.8340784336131711, 0.8305958991958129, 0.827080574274562, 0.8235325976284276, 0.8199521093254525, 0.816339250717184, 0.8126941644330941, 0.8090169943749475, 0.805307885711122, 0.8015669848708769, 0.7977944395385712, 0.7939903986478355, 0.7901550123756905, 0.786288432136619, 0.7823908105765882, 0.7784623015670235, 0.7745030601987338, 0.7705132427757893, 0.7664930068093498, 0.7624425110114481, 0.758361915288722, 0.754251380736104, 0.7501110696304596, 0.7459411454241822, 0.7417417727387393, 0.7375131173581739, 0.73325534622256, 0.7289686274214114, 0.7246531301870468, 0.720309024887907, 0.7159364830218313, 0.7115356772092855, 0.7071067811865476, 0.7026499697988492, 0.6981654189934726, 0.6936533058128049, 0.6891138083873484, 0.6845471059286888, 0.6799533787224193, 0.6753328081210246, 0.6706855765367201, 0.6660118674342517, 0.6613118653236518, 0.6565857557529564, 0.6518337253008787, 0.6470559615694442, 0.6422526531765845, 0.6374239897486899, 0.6325701619131245, 0.6276913612907006, 0.6227877804881126, 0.6178596130903343, 0.6129070536529764, 0.6079302976946053, 0.6029295416890246, 0.5979049830575187, 0.5928568201610593, 0.5877852522924732, 0.5826904796685761, 0.5775727034222676, 0.5724321255945908, 0.5672689491267564, 0.5620833778521305, 0.5568756164881878, 0.5516458706284304, 0.5463943467342692, 0.5411212521268759, 0.535826794978997, 0.5305111843067344, 0.525174629961296, 0.5198173426207098, 0.5144395337815066, 0.5090414157503714, 0.5036232016357609, 0.4981851053394909, 0.49272734154829156, 0.48725012572533266, 0.4817536741017156, 0.4762382036679394, 0.4707039321653328, 0.46515107807745854, 0.459579860621488, 0.45399049973954686, 0.4483832160900323, 0.44275823103890155, 0.4371157666509329, 0.4314560456809593, 0.4257792915650729, 0.42008572841180647, 0.4143755809932843, 0.40864907473634915, 0.40290643571366275, 0.3971478906347806, 0.39137366683720237, 0.3855839922773965, 0.3797790955218014, 0.37395920573780067, 0.36812455268467814, 0.3622753667045458, 0.3564118787132508, 0.350534320191259, 0.34464292317451706, 0.3387379202452913, 0.3328195445229865, 0.32688802965494274, 0.3209436098072097, 0.31498651965530494, 0.3090169943749475, 0.30303526963277405, 0.2970415815770349, 0.29103616682827177, 0.28501926246997605, 0.2789911060392291, 0.27295193551732505, 0.26690198932037573, 0.26084150628989705, 0.25477072568338227, 0.24868988716485482, 0.2425992307954074, 0.2364989970237246, 0.23038942667659046, 0.224270760949381, 0.21814324139654231, 0.2120071099220548, 0.2058626087698814, 0.19970998051440705, 0.19354946805086068, 0.18738131458572502, 0.18120576362713767, 0.1750230589752763, 0.1688334447127341, 0.16263716519488378, 0.15643446504023098, 0.15022558912075712, 0.1440107825522526, 0.13779029068463847, 0.13156435909228284, 0.12533323356430454, 0.11909716009486998, 0.11285638487348187, 0.10661115427526005, 0.10036171485121498, 0.09410831331851435, 0.08785119655074315, 0.08159061156815792, 0.07532680552793304, 0.06906002571440606, 0.06279051952931358, 0.05651853448202468, 0.05024431817976966, 0.04396811831786495, 0.037690182669934534, 0.031410759078128236, 0.025130095443337813, 0.01884843971540846, 0.012566039883352836, 0.006283143965559127, 1.2246467991473532e-16, -0.006283143965558882, -0.012566039883352592, -0.018848439715408213, -0.02513009544333757, -0.031410759078127994, -0.03769018266993429, -0.04396811831786471, -0.05024431817976942, -0.056518534482024436, -0.06279051952931335, -0.06906002571440581, -0.07532680552793279, -0.08159061156815768, -0.0878511965507429, -0.0941083133185141, -0.10036171485121473, -0.1066111542752598, -0.11285638487348164, -0.11909716009486973, -0.12533323356430429, -0.13156435909228262, -0.13779029068463822, -0.14401078255225236, -0.15022558912075687, -0.15643446504023073, -0.16263716519488353, -0.16883344471273384, -0.1750230589752761, -0.18120576362713745, -0.18738131458572477, -0.19354946805086046, -0.19970998051440725, -0.2058626087698812, -0.21200710992205454, -0.2181432413965425, -0.2242707609493812, -0.23038942667659065, -0.2364989970237248, -0.2425992307954076, -0.24868988716485502, -0.25477072568338244, -0.26084150628989683, -0.2669019893203755, -0.2729519355173252, -0.2789911060392293, -0.2850192624699762, -0.291036166828272, -0.2970415815770351, -0.3030352696327742, -0.30901699437494773, -0.31498651965530516, -0.3209436098072095, -0.3268880296549425, -0.33281954452298673, -0.3387379202452915, -0.3446429231745172, -0.35053432019125924, -0.356411878713251, -0.362275366704546, -0.3681245526846783, -0.37395920573780045, -0.37977909552180117, -0.38558399227739626, -0.39137366683720215, -0.39714789063478, -0.4029064357136621, -0.40864907473634854, -0.41437558099328364, -0.42008572841180586, -0.42577929156507227, -0.43145604568095863, -0.43711576665093266, -0.44275823103890133, -0.4483832160900317, -0.45399049973954625, -0.45957986062148737, -0.4651510780774579, -0.4707039321653322, -0.4762382036679388, -0.481753674101715, -0.4872501257253321, -0.4927273415482914, -0.4981851053394907, -0.5036232016357604, -0.5090414157503709, -0.5144395337815061, -0.5198173426207091, -0.5251746299612954, -0.5305111843067338, -0.5358267949789964, -0.5411212521268757, -0.546394346734269, -0.5516458706284298, -0.5568756164881876, -0.5620833778521303, -0.5672689491267562, -0.5724321255945907, -0.5775727034222674, -0.5826904796685759, -0.587785252292473, -0.5928568201610591, -0.5979049830575185, -0.6029295416890244, -0.607930297694605, -0.6129070536529763, -0.6178596130903341, -0.6227877804881123, -0.6276913612907004, -0.6325701619131244, -0.6374239897486896, -0.6422526531765844, -0.647055961569444, -0.6518337253008785, -0.6565857557529562, -0.6613118653236517, -0.6660118674342514, -0.6706855765367199, -0.6753328081210244, -0.6799533787224191, -0.6845471059286887, -0.6891138083873481, -0.6936533058128047, -0.6981654189934725, -0.7026499697988491, -0.7071067811865475, -0.7115356772092852, -0.7159364830218311, -0.7203090248879069, -0.7246531301870467, -0.7289686274214113, -0.7332553462225598, -0.7375131173581737, -0.7417417727387391, -0.7459411454241821, -0.7501110696304595, -0.7542513807361038, -0.7583619152887215, -0.7624425110114479, -0.7664930068093496, -0.7705132427757894, -0.7745030601987337, -0.7784623015670236, -0.7823908105765881, -0.7862884321366186, -0.7901550123756904, -0.793990398647835, -0.7977944395385711, -0.8015669848708764, -0.8053078857111221, -0.8090169943749473, -0.8126941644330942, -0.8163392507171839, -0.819952109325452, -0.8235325976284275, -0.8270805742745616, -0.8305958991958127, -0.834078433613171, -0.8375280400421419, -0.840944582298169, -0.8443279255020153, -0.8476779360850832, -0.8509944817946921, -0.8542774316992952, -0.8575266561936521, -0.8607420270039438, -0.8639234171928352, -0.8670707011644903, -0.8701837546695256, -0.8732624548099204, -0.8763066800438636, -0.8793163101905564, -0.8822912264349534, -0.8852313113324551, -0.8881364488135446, -0.8910065241883678, -0.8938414241512639, -0.8966410367852359, -0.8994052515663712, -0.9021339593682028, -0.9048270524660198, -0.907484424541117, -0.9101059706849955, -0.9126915874035029, -0.9152411726209175, -0.9177546256839813, -0.9202318473658704, -0.922672739870115, -0.9250772068344577, -0.9274451533346612, -0.9297764858882511, -0.9320711124582108, -0.9343289424566118, -0.9365498867481923, -0.938733857653874, -0.9408807689542251, -0.9429905358928643, -0.9450630751798046, -0.9470983049947442, -0.9490961449902944, -0.9510565162951535, -0.9529793415172186, -0.954864544746643, -0.9567120515588303, -0.9585217890173756, -0.960293685676943, -0.9620276715860857, -0.9637236782900096, -0.9653816388332738, -0.967001487762435, -0.968583161128631, -0.9701265964901058, -0.9716317329146739, -0.9730985109821264, -0.9745268727865771, -0.9759167619387472, -0.9772681235681934, -0.978580904325472, -0.9798550523842469, -0.981090517443334, -0.9822872507286887, -0.9834452049953296, -0.9845643345292054, -0.9856445951489979, -0.9866859442078679, -0.9876883405951377, -0.988651744737914, -0.9895761186026509, -0.9904614256966512, -0.9913076310695066, -0.9921147013144778, -0.9928826045698137, -0.9936113105200084, -0.9943007903969988, -0.9949510169813002, -0.99556196460308, -0.9961336091431725, -0.9966659280340299, -0.9971589002606139, -0.9976125063612252, -0.9980267284282716, -0.998401550108975, -0.9987369566060175, -0.9990329346781247, -0.9992894726405892, -0.9995065603657316, -0.9996841892832999, -0.999822352380809, -0.9999210442038161, -0.9999802608561371, -1.0, -0.9999802608561371, -0.9999210442038161, -0.999822352380809, -0.9996841892832999, -0.9995065603657316, -0.9992894726405892, -0.9990329346781247, -0.9987369566060175, -0.998401550108975, -0.9980267284282716, -0.9976125063612252, -0.9971589002606139, -0.9966659280340299, -0.9961336091431725, -0.99556196460308, -0.9949510169813002, -0.9943007903969988, -0.9936113105200084, -0.9928826045698137, -0.9921147013144779, -0.9913076310695066, -0.9904614256966512, -0.989576118602651, -0.988651744737914, -0.9876883405951378, -0.986685944207868, -0.985644595148998, -0.9845643345292054, -0.9834452049953297, -0.9822872507286887, -0.9810905174433341, -0.979855052384247, -0.9785809043254721, -0.9772681235681935, -0.9759167619387473, -0.9745268727865771, -0.9730985109821264, -0.971631732914674, -0.9701265964901059, -0.9685831611286311, -0.9670014877624351, -0.9653816388332738, -0.9637236782900097, -0.9620276715860858, -0.9602936856769431, -0.9585217890173757, -0.9567120515588304, -0.9548645447466431, -0.9529793415172187, -0.9510565162951536, -0.9490961449902945, -0.9470983049947443, -0.9450630751798047, -0.9429905358928644, -0.9408807689542253, -0.9387338576538741, -0.9365498867481924, -0.9343289424566119, -0.9320711124582111, -0.9297764858882512, -0.9274451533346614, -0.9250772068344579, -0.9226727398701151, -0.9202318473658705, -0.9177546256839815, -0.9152411726209176, -0.9126915874035031, -0.9101059706849958, -0.9074844245411171, -0.9048270524660199, -0.9021339593682031, -0.8994052515663714, -0.896641036785236, -0.8938414241512641, -0.891006524188368, -0.8881364488135448, -0.8852313113324553, -0.8822912264349535, -0.8793163101905567, -0.8763066800438638, -0.8732624548099205, -0.8701837546695258, -0.8670707011644904, -0.8639234171928354, -0.8607420270039439, -0.8575266561936523, -0.8542774316992954, -0.8509944817946923, -0.8476779360850835, -0.8443279255020155, -0.8409445822981693, -0.8375280400421421, -0.8340784336131712, -0.8305958991958129, -0.8270805742745618, -0.8235325976284277, -0.8199521093254523, -0.8163392507171842, -0.8126941644330944, -0.8090169943749476, -0.8053078857111223, -0.8015669848708766, -0.7977944395385713, -0.7939903986478354, -0.7901550123756906, -0.7862884321366188, -0.7823908105765883, -0.7784623015670239, -0.7745030601987339, -0.7705132427757896, -0.7664930068093498, -0.7624425110114482, -0.7583619152887218, -0.754251380736104, -0.7501110696304595, -0.7459411454241823, -0.7417417727387396, -0.737513117358174, -0.7332553462225604, -0.7289686274214116, -0.7246531301870469, -0.7203090248879068, -0.7159364830218313, -0.7115356772092852, -0.7071067811865477, -0.7026499697988496, -0.6981654189934727, -0.6936533058128054, -0.6891138083873485, -0.684547105928689, -0.6799533787224191, -0.6753328081210246, -0.6706855765367199, -0.6660118674342518, -0.6613118653236523, -0.6565857557529565, -0.6518337253008791, -0.6470559615694443, -0.6422526531765846, -0.6374239897486896, -0.6325701619131247, -0.6276913612907002, -0.6227877804881126, -0.6178596130903348, -0.6129070536529765, -0.6079302976946057, -0.6029295416890247, -0.5979049830575192, -0.5928568201610591, -0.5877852522924734, -0.5826904796685758, -0.5775727034222677, -0.5724321255945913, -0.5672689491267565, -0.5620833778521309, -0.556875616488188, -0.5516458706284305, -0.5463943467342689, -0.541121252126876, -0.5358267949789963, -0.5305111843067342, -0.5251746299612954, -0.5198173426207094, -0.5144395337815068, -0.5090414157503712, -0.503623201635761, -0.4981851053394906, -0.4927273415482917, -0.487250125725332, -0.4817536741017153, -0.47623820366793873, -0.4707039321653325, -0.46515107807745865, -0.4595798606214877, -0.45399049973954697, -0.448383216090032, -0.44275823103890166, -0.43711576665093255, -0.43145604568095897, -0.4257792915650722, -0.4200857284118062, -0.4143755809932844, -0.40864907473634887, -0.40290643571366286, -0.39714789063478034, -0.3913736668372025, -0.38558399227739615, -0.3797790955218019, -0.3739592057378008, -0.3681245526846787, -0.3622753667045459, -0.3564118787132513, -0.3505343201912592, -0.34464292317451756, -0.33873792024529226, -0.33281954452298707, -0.3268880296549433, -0.3209436098072098, -0.3149865196553055, -0.3090169943749477, -0.30303526963277455, -0.29704158157703503, -0.2910361668282723, -0.285019262469977, -0.27899110603922966, -0.272951935517326, -0.26690198932037584, -0.2608415062898976, -0.2547707256833824, -0.24868988716485535, -0.24259923079540752, -0.23649899702372515, -0.23038942667659143, -0.22427076094938156, -0.2181432413965433, -0.2120071099220549, -0.20586260876988197, -0.1997099805144072, -0.19354946805086082, -0.18738131458572468, -0.1812057636271378, -0.175023058975276, -0.1688334447127342, -0.16263716519488433, -0.1564344650402311, -0.15022558912075767, -0.1440107825522523, -0.13779029068463858, -0.13156435909228253, -0.12533323356430465, -0.11909716009486966, -0.112856384873482, -0.1066111542752606, -0.10036171485121509, -0.09410831331851491, -0.08785119655074328, -0.08159061156815804, -0.07532680552793272, -0.06906002571440618, -0.06279051952931326, -0.056518534482024804, -0.05024431817977022, -0.043968118317865075, -0.0376901826699351, -0.03141075907812836, -0.025130095443337937, -0.018848439715408137, -0.01256603988335296, -0.006283143965558805}

typedef enum PID_type
{
	positional,
	increment,
}PID_type;

typedef struct
{
	float ref;//输入：系统待调节量的给定值
	float fdb;//输入：系统待调节量的反馈值
	float inte;//积分值
	float err[3];
	float kp;
	float ki;
	float kd;

	float componentKiMax;
	float deadZone;

	float output;
	float outputMax;
	PID_type type;
}PID_Regulator_t;


extern PID_Regulator_t cloud_pitch_speed_pid;
extern PID_Regulator_t cloud_pitch_position_pid;
extern PID_Regulator_t cloud_yaw_speed_pid;
extern PID_Regulator_t cloud_yaw_position_pid;
extern PID_Regulator_t underpan_motor[4];
extern PID_Regulator_t dan_pid;
extern float mySin[1000];

void PID_Calc(PID_Regulator_t *pid, float ref, float fdb);
void PID_Init(PID_Regulator_t *pid,float kp,float ki,float kd,float componentKiMax, float deadZone, float outputMax,PID_type type);
void Cloud_Speed(void);
void Cloud_Position(void);
void ALLPID_Init(void);




#endif
