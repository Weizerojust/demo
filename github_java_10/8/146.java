package programmieren;

/**
 * Can caluclate the Levenshtein-Distance of two Strings.
 * Class is meant for static usage, the sole constructor is private.
 *
 * @author Markus Iser
 * @version 1.2
 */
public class Levenshtein {

    private final String w1;
    private final String w2;
    private final int[][] distance;

    /**
     * Crate a new Levenshtein distance object.
     * @param w1 The first word.
     * @param w2 The second word.
     */
    public Levenshtein(String w1, String w2) {
        this.w1 = "." + w1.toLowerCase();
        this.w2 = "." + w2.toLowerCase();
        this.distance = new int[this.w2.length()][this.w1.length()];
    }

    /**
     * @return w1 The first word.
     *
     */
    public String getFirstWord() {
        return w1;
    }

    /**
     * @return w2 The second word.
     *
     */
    public String getSecondWord() {
        return w2;
    }

    /**
     * @return The Levenshtein distance of the two words.
     */
    public double getDistance() {
        distance[0][0] = 0;
        for (int i = 0; i < w2.length(); i++) {
            for (int j = 0; j < w1.length(); j++) {
                if (i != 0 || j != 0) {
                    distance[i][j] = Math.min(replace(i, j),
                                     Math.min(delete(i, j), insert(i, j)));
                }
            }
        }
        return distance[w2.length() - 1][w1.length() - 1];
    }

    private int getDistanceAt(int i, int j) {
        if (i < 0 || j < 0 || i > w2.length() || j > w1.length()) {
            return Integer.MAX_VALUE - 1;
        }
        return distance[i][j];
    }

    private int delete(int i, int j) {
        return getDistanceAt(i, j - 1) + 1;
    }

    private int insert(int i, int j) {
        return getDistanceAt(i - 1, j) + 1;
    }

    private int replace(int i, int j) {
        return w2.charAt(i) == w1.charAt(j) ? getDistanceAt(i - 1, j - 1) : getDistanceAt(i - 1, j - 1) + 1;
    }
}